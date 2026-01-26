#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include "FCLayer.h"
#include "NeuralNetwork.cpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "aggregations.h" 
#include "attacks.h"

using namespace std;

int main_sim(bool perf, bool tf, bool custom, bool pretrained,
             int fl_devices, int local_episodes, int batch_size, int epochs, const string& attack_type, const string& aggregation_type, double prune_percentage) {
    int input_size = 256;
    double quant_scale = 0.04379776492714882;
    int quant_zero_point = -128;
    int output_size = 2;
    int val_batches = max(200, min(2000 / fl_devices, batch_size * 10));

    assert(8000 >= epochs * fl_devices * batch_size);
    assert(2000 >= val_batches * fl_devices);
    assert(val_batches % batch_size == 0);

    std::string embedding_data = "Data/train_embeddings.txt";
    std::string gt_data = "Data/train_ground_truth.txt";
    std::string embedding_data_val = "Data/test_embeddings.txt";
    std::string gt_data_val = "Data/test_ground_truth.txt";

    ifstream data_file(embedding_data);
    ifstream gt_file(gt_data);
    if (!data_file.is_open()) throw runtime_error("Could not open data file");
    if (!gt_file.is_open()) throw runtime_error("Could not open gt file");

    std::string line_data;
    std::string line_gt;

    double dropout_rate = 0.15;
    FCLayer* devices = new FCLayer[fl_devices];
    for (int d = 0; d < fl_devices; d++) {
        devices[d] = FCLayer(input_size, output_size, quant_scale, quant_zero_point, batch_size, pretrained, dropout_rate);
    }

    double ***input_data = new double**[fl_devices];
    int ***ground_truth = new int**[fl_devices];
    double ***input_data_val = new double**[fl_devices];
    int ***ground_truth_val = new int**[fl_devices];

    for (int d = 0; d < fl_devices; d++) {
        input_data[d] = new double*[batch_size];
        ground_truth[d] = new int*[batch_size];
        input_data_val[d] = new double*[val_batches];
        ground_truth_val[d] = new int*[val_batches];

        for (int b = 0; b < batch_size; b++) {
            input_data[d][b] = new double[input_size];
            ground_truth[d][b] = new int[output_size];
            fill(input_data[d][b], input_data[d][b] + input_size, 0.0);
            fill(ground_truth[d][b], ground_truth[d][b] + output_size, 0);
        }

        for (int b = 0; b < val_batches; b++) {
            input_data_val[d][b] = new double[input_size];
            ground_truth_val[d][b] = new int[output_size];
            fill(input_data_val[d][b], input_data_val[d][b] + input_size, 0.0);
            fill(ground_truth_val[d][b], ground_truth_val[d][b] + output_size, 0);
        }
    }

    double **output_val = new double*[val_batches];
    for (int b = 0; b < val_batches; b++) {
        output_val[b] = new double[output_size];
        fill(output_val[b], output_val[b] + output_size, 0.0);
    }

    double **server_weights = new double*[input_size];
    double *server_bias = new double[output_size];
    for (int i = 0; i < input_size; i++) {
        server_weights[i] = new double[output_size];
        fill(server_weights[i], server_weights[i] + output_size, 0.0);
    }
    fill(server_bias, server_bias + output_size, 0.0);

    double data_val;
    int gt_val;
    double learning_rate = 0.01;
    double lambda = 0.0001;

    for (int epoch = 0; epoch < epochs; epoch++) {
        for (int d = 0; d < fl_devices; d++) {
            FCLayer* NNmodel = &(devices[d]);
            for (int b = 0; b < batch_size; b++) {
                getline(data_file, line_data);
                getline(gt_file, line_gt);
                stringstream ss_data(line_data);
                stringstream ss_gt(line_gt);

                for (int i = 0; i < input_size; i++) {
                    ss_data >> data_val;
                    input_data[d][b][i] = data_val;
                }
                ss_gt >> gt_val;
                fill(ground_truth[d][b], ground_truth[d][b] + output_size, 0);
                ground_truth[d][b][gt_val] = 1;
            }
            cout << "epoch: " << epoch << " device: " << d << " ";
            FL_round_simulation(input_data[d], nullptr, ground_truth[d], local_episodes, learning_rate, 
                                NNmodel, lambda, false, false, false);

            ifstream data_file_val(embedding_data_val);
            ifstream gt_file_val(gt_data_val);
            if (!data_file_val.is_open()) throw runtime_error("Could not open data file");
            if (!gt_file_val.is_open()) throw runtime_error("Could not open gt file");

            double acc = 0.0;
            for (int v = 0; v < val_batches / batch_size; v++) {
                for (int b = 0; b < batch_size; b++) {
                    getline(data_file_val, line_data);
                    getline(gt_file_val, line_gt);
                    stringstream ss_data(line_data);
                    stringstream ss_gt(line_gt);

                    for (int i = 0; i < input_size; i++) {
                        ss_data >> data_val;
                        input_data_val[d][b][i] = data_val;
                    }
                    ss_gt >> gt_val;
                    fill(ground_truth_val[d][b], ground_truth_val[d][b] + output_size, 0);
                    ground_truth_val[d][b][gt_val] = 1;
                }
                predict(input_data_val[d], NNmodel, output_val, batch_size, output_size);
                acc += accuracy(output_val, ground_truth_val[d], batch_size, output_size) / (val_batches / batch_size);
            }
            cout << " val acc: " << acc << "\n";
        }

        std::fill(server_bias, server_bias + output_size, 0.0);
        for (int i = 0; i < input_size; i++) {
            std::fill(server_weights[i], server_weights[i] + output_size, 0.0);
        }

        simulateAttack(devices, fl_devices, input_size, output_size, attack_type);
        
        simulateAggregation(devices, server_weights, server_bias, fl_devices, input_size, output_size, aggregation_type);

        prune_weights_hard(server_weights, prune_percentage, input_size, output_size);

        for (int d = 0; d < fl_devices; d++) {
            devices[d].set_weights_bias(server_weights, server_bias);
        }

        learning_rate = 0.01 * pow(0.96, (epoch * batch_size / 1000));
    }

    data_file.close();
    gt_file.close();

    for (int i = 0; i < input_size; i++) {
        delete[] server_weights[i];
    }
    delete[] server_weights;
    delete[] server_bias;

    for (int d = 0; d < fl_devices; d++) {
        devices[d].cleanup();
    }

    for (int b = 0; b < val_batches; b++) {
        delete[] output_val[b];
    }
    delete[] output_val;

    for (int d = 0; d < fl_devices; d++) {
        for (int b = 0; b < batch_size; b++) {
            delete[] input_data[d][b];
            delete[] ground_truth[d][b];
            delete[] input_data_val[d][b];
            delete[] ground_truth_val[d][b];
        }
        delete[] input_data[d];
        delete[] ground_truth[d];
        delete[] input_data_val[d];
        delete[] ground_truth_val[d];
    }
    delete[] input_data;
    delete[] ground_truth;
    delete[] input_data_val;
    delete[] ground_truth_val;
    delete[] devices; 
    return 0;
}

void redirect_stdout(const string &filename) {
    freopen(filename.c_str(), "w", stdout);
}

int main(int argc, char **argv) {
    int fl_devices = 5;
    int local_episodes = 15; 
    int batch_size = 10; 
    int epochs = 8000 / (batch_size * fl_devices);
    double prune_percentage = 0.15; 

    vector<string> attack_types = {"noByz","trimAttack","krumAttack","fltrustAttack", "minMaxAttack", "minSumAttack", "scalingAttack"};
    vector<string> aggregation_types = {"federatedAveraging","trimMean","krum", "median","fltrust", "romoa"};

    for (const auto& attack_type : attack_types) {
        for (const auto& aggregation_type : aggregation_types) {
            string filename = attack_type + "_" + aggregation_type + ".txt";
            redirect_stdout(filename);

            cout << "DEFAULT SIMULATION\n";
            for (int trials = 0; trials < 1; trials++) {
                cout << "============================\n";
                cout << "TRIAL: " << trials << "\n";
                cout << "START: fl_devices " << fl_devices << " local_episodes " << local_episodes << " batch_size " << batch_size << "\n";

                main_sim(true, false, false, false, fl_devices, local_episodes, batch_size, epochs, attack_type, aggregation_type, prune_percentage);

                cout << "END TRIAL\n";
            }
        }
    }

    return 0;
}
