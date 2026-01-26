#include "aggregations.h"
#include "FCLayer.h"
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include <numeric>
#include <random>
#include <cmath>

// Federated Averaging
void federatedAveraging(FCLayer* devices, double** server_weights, double* server_bias, int fl_devices, int input_size, int output_size) {
    for (int i = 0; i < input_size; i++) {
        for (int j = 0; j < output_size; j++) {
            for (int d = 0; d < fl_devices; d++) {
                server_weights[i][j] += devices[d].weights[i][j] / fl_devices;
                server_bias[j] += devices[d].bias[j] / (input_size * fl_devices);
            }
        }
    }
}

// Trim Mean
double trimMean(int arr[], int n, double trim_fraction) {
    int trim_count = static_cast<int>(n * trim_fraction);
    std::sort(arr, arr + n);
    int sum = 0;
    for (int i = trim_count; i < n - trim_count; ++i) {
        sum += arr[i];
    }
    int trimmed_size = n - 2 * trim_count;
    return static_cast<double>(sum) / trimmed_size;
}


void aggregateWithTrimMean(double** server_weights, double* server_bias, FCLayer* devices, int input_size, int output_size, int fl_devices) {
    double trim_fraction = 0.1;
    for (int i = 0; i < input_size; i++) {
        for (int j = 0; j < output_size; j++) {
            std::vector<int> weights(fl_devices);
            for (int d = 0; d < fl_devices; d++) {
                weights[d] = static_cast<int>(devices[d].weights[i][j] * 1000);
            }
            server_weights[i][j] = trimMean(weights.data(), fl_devices, trim_fraction) / 1000.0;
        }
    }

    for (int j = 0; j < output_size; j++) {
        std::vector<int> biases(fl_devices);
        for (int d = 0; d < fl_devices; d++) {
            biases[d] = static_cast<int>(devices[d].bias[j] * 1000);
        }
        server_bias[j] = trimMean(biases.data(), fl_devices, trim_fraction) / 1000.0;
    }
}

// Krum
void krum(double** weights, double* biases, FCLayer* devices, int fl_devices, int input_size, int output_size) {
    int f = 1;
    std::vector<double> distances(fl_devices, 0.0);
    for (int i = 0; i < fl_devices; ++i) {
        std::vector<double> dist(fl_devices, 0.0);
        for (int j = 0; j < fl_devices; ++j) {
            if (i != j) {
                double sum = 0.0;
                for (int k = 0; k < input_size; ++k) {
                    for (int l = 0; l < output_size; ++l) {
                        sum += std::pow(devices[i].weights[k][l] - devices[j].weights[k][l], 2);
                    }
                }
                dist[j] = std::sqrt(sum);
            }
        }
        std::sort(dist.begin(), dist.end());
        distances[i] = std::accumulate(dist.begin(), dist.begin() + (fl_devices - f - 1), 0.0);
    }
    int chosen = std::min_element(distances.begin(), distances.end()) - distances.begin();
    for (int i = 0; i < input_size; ++i) {
        for (int j = 0; j < output_size; ++j) {
            weights[i][j] = devices[chosen].weights[i][j];
        }
    }
    for (int j = 0; j < output_size; ++j) {
        biases[j] = devices[chosen].bias[j];
    }
}

// Median
void median(double** weights, double* biases, FCLayer* devices, int fl_devices, int input_size, int output_size) {
    for (int i = 0; i < input_size; i++) {
        for (int j = 0; j < output_size; j++) {
            std::vector<double> weight_values(fl_devices);
            for (int d = 0; d < fl_devices; d++) {
                weight_values[d] = devices[d].weights[i][j];
            }
            std::nth_element(weight_values.begin(), weight_values.begin() + fl_devices / 2, weight_values.end());
            weights[i][j] = weight_values[fl_devices / 2];
        }
    }

    for (int j = 0; j < output_size; j++) {
        std::vector<double> bias_values(fl_devices);
        for (int d = 0; d < fl_devices; d++) {
            bias_values[d] = devices[d].bias[j];
        }
        std::nth_element(bias_values.begin(), bias_values.begin() + fl_devices / 2, bias_values.end());
        biases[j] = bias_values[fl_devices / 2];
    }
}

// FLTrust
void fltrust(double** weights, double* biases, FCLayer* devices, int fl_devices, int input_size, int output_size) {
    std::vector<double> trust_scores(fl_devices, 0.0);
    std::vector<double> baseline_weights(input_size * output_size, 0.0);

    for (int i = 0; i < input_size; ++i) {
        for (int j = 0; j < output_size; ++j) {
            baseline_weights[i * output_size + j] = devices[fl_devices - 1].weights[i][j];
        }
    }

    for (int d = 0; d < fl_devices - 1; ++d) {
        double dot_product = 0.0, norm_d = 0.0, norm_b = 0.0;
        for (int i = 0; i < input_size; ++i) {
            for (int j = 0; j < output_size; ++j) {
                dot_product += devices[d].weights[i][j] * baseline_weights[i * output_size + j];
                norm_d += std::pow(devices[d].weights[i][j], 2);
                norm_b += std::pow(baseline_weights[i * output_size + j], 2);
            }
        }
        double denominator = (std::sqrt(norm_d) * std::sqrt(norm_b));
        if (denominator == 0) {
            trust_scores[d] = 0.0;
        } else {
            trust_scores[d] = std::max(0.0, dot_product / denominator);
        }
    }

    double sum_trust_scores = std::accumulate(trust_scores.begin(), trust_scores.end(), 0.0);
    if (sum_trust_scores > 0) {
        for (int d = 0; d < fl_devices - 1; ++d) {
            trust_scores[d] /= sum_trust_scores;
        }
    }

    for (int d = 0; d < fl_devices - 1; ++d) {
        trust_scores[d] = std::min(1.0, std::max(0.0, trust_scores[d]));
    }

    for (int i = 0; i < input_size; ++i) {
        for (int j = 0; j < output_size; ++j) {
            weights[i][j] = 0.0;
            for (int d = 0; d < fl_devices - 1; ++d) {
                weights[i][j] += trust_scores[d] * devices[d].weights[i][j];
            }
        }
    }

    for (int j = 0; j < output_size; ++j) {
        biases[j] = 0.0;
        for (int d = 0; d < fl_devices - 1; ++d) {
            biases[j] += trust_scores[d] * devices[d].bias[j];
        }
    }
}

// ROMOA
void romoa(double** weights, double* biases, FCLayer* devices, int fl_devices, int input_size, int output_size) {
    double F[fl_devices] = {0}; 
    double prev_global_update[input_size * output_size] = {0}; 
    int seed = 123; 
    std::mt19937 gen(seed);
    std::uniform_real_distribution<> dis(0, 1);

    int num_params = input_size * output_size;
    std::vector<int> selected_params;
    for (int i = 0; i < num_params; ++i) {
        if (dis(gen) < 0.1) {
            selected_params.push_back(i);
        }
    }

    std::vector<double> distances(fl_devices, 0.0);
    for (int d = 0; d < fl_devices; ++d) {
        for (int p : selected_params) {
            distances[d] += std::pow(devices[d].weights[p / output_size][p % output_size] - prev_global_update[p], 2);
        }
        distances[d] = std::sqrt(distances[d]);
    }

    std::nth_element(distances.begin(), distances.begin() + fl_devices / 2, distances.end());
    double median_distance = distances[fl_devices / 2];

    for (int d = 0; d < fl_devices; ++d) {
        F[d] = std::exp(-distances[d] / median_distance);
    }

    double sum_F = std::accumulate(F, F + fl_devices, 0.0);

    for (int i = 0; i < input_size; ++i) {
        for (int j = 0; j < output_size; ++j) {
            double global_update_ij = 0.0;
            for (int d = 0; d < fl_devices; ++d) {
                global_update_ij += devices[d].weights[i][j] * F[d];
            }
            weights[i][j] = global_update_ij / sum_F;
        }
    }

    for (int j = 0; j < output_size; ++j) {
        double global_update_bias_j = 0.0;
        for (int d = 0; d < fl_devices; ++d) {
            global_update_bias_j += devices[d].bias[j] * F[d];
        }
        biases[j] = global_update_bias_j / sum_F;
    }
}

void simulateAggregation(FCLayer* devices, double** server_weights, double* server_bias, int fl_devices, int input_size, int output_size, const std::string& aggregation_type) {
    if (aggregation_type == "federatedAveraging") {
        federatedAveraging(devices, server_weights, server_bias, fl_devices, input_size, output_size);
    } else if (aggregation_type == "trimMean") {
        aggregateWithTrimMean(server_weights, server_bias, devices, input_size, output_size, fl_devices);
    } else if (aggregation_type == "krum") {
        krum(server_weights, server_bias, devices, fl_devices, input_size, output_size);
    } else if (aggregation_type == "median") {
        median(server_weights, server_bias, devices, fl_devices, input_size, output_size);
    } else if (aggregation_type == "fltrust") {
        fltrust(server_weights, server_bias, devices, fl_devices, input_size, output_size);
    } else if (aggregation_type == "romoa") {
        romoa(server_weights, server_bias, devices, fl_devices, input_size, output_size);
    } else {
        std::cerr << "Unknown aggregation type: " << aggregation_type << std::endl;
    }
}
