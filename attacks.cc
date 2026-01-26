#include "attacks.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

void no_byz(double** weights, int input_size, int output_size) {
}

void trim_attack(double** weights, int input_size, int output_size) {
    for (int i = 0; i < input_size; i++) {
        double max_val = *std::max_element(weights[i], weights[i] + output_size);
        double min_val = *std::min_element(weights[i], weights[i] + output_size);
        double direction = std::accumulate(weights[i], weights[i] + output_size, 0.0) > 0 ? 1.0 : -1.0;
        double directed_val = direction > 0 ? min_val : max_val;
        
        double random_val = static_cast<double>(rand()) / RAND_MAX + 1.0;
        for (int j = 0; j < output_size; j++) {
            weights[i][j] = directed_val * random_val; 
        }
    }
}

void krum_attack(double** weights, int input_size, int output_size) {
    double attack_strength = static_cast<double>(rand()) / RAND_MAX; 

    for (int i = 0; i < input_size; i++) {
        for (int j = 0; j < output_size; j++) {
            weights[i][j] = -attack_strength * std::copysign(1.0, weights[i][j]);
        }
    }
}

void fltrust_attack(double** weights, int input_size, int output_size) {
    double attack_strength = static_cast<double>(rand()) / RAND_MAX; 

    for (int i = 0; i < input_size; i++) {
        double norm = 0.0;
        for (int j = 0; j < output_size; j++) {
            norm += std::pow(weights[i][j], 2);
        }
        norm = std::sqrt(norm);

        double norm_factor = norm > 1.0 ? 1.0 / norm : 1.0;
        for (int j = 0; j < output_size; j++) {
            weights[i][j] *= norm_factor;
            weights[i][j] *= attack_strength;
        }
    }
}

void min_max_attack(double** weights, int input_size, int output_size) {
    double gamma = static_cast<double>(rand()) / RAND_MAX; 

    for (int i = 0; i < input_size; i++) {
        double mean = std::accumulate(weights[i], weights[i] + output_size, 0.0) / output_size;

        double deviation = 0.0;
        for (int j = 0; j < output_size; j++) {
            deviation += std::pow(weights[i][j] - mean, 2);
        }
        deviation = std::sqrt(deviation / output_size);

        for (int j = 0; j < output_size; j++) {
            weights[i][j] = mean - gamma * deviation;
        }
    }
}

void min_sum_attack(double** weights, int input_size, int output_size) {
    double gamma = static_cast<double>(rand()) / RAND_MAX; 

    for (int i = 0; i < input_size; i++) {
        double mean = std::accumulate(weights[i], weights[i] + output_size, 0.0) / output_size;

        double sum = 0.0;
        for (int j = 0; j < output_size; j++) {
            sum += std::pow(weights[i][j] - mean, 2);
        }

        double adjustment = gamma * (sum / output_size);
        for (int j = 0; j < output_size; j++) {
            weights[i][j] = mean - adjustment;
        }
    }
}

void scaling_attack(double** weights, int input_size, int output_size) {
    double scaling_factor = (rand() % 20 + 1) * (rand() % 2 == 0 ? 1 : -1);  
    for (int i = 0; i < input_size; i++) {
        for (int j = 0; j < output_size; j++) {
            weights[i][j] *= scaling_factor;
        }
    }
}

void normalize_weights(double** weights, int input_size, int output_size) {
    double max_val = 1.0; 
    double min_val = -1.0;
    for (int i = 0; i < input_size; i++) {
        for (int j = 0; j < output_size; j++) {
            if (weights[i][j] > max_val) {
                weights[i][j] = max_val;
            } else if (weights[i][j] < min_val) {
                weights[i][j] = min_val;
            }
        }
    }
}

void simulateAttack(FCLayer* devices, int fl_devices, int input_size, int output_size, const std::string& attack_type) {
    double malicious_ratio = 0.45;
    int num_malicious = static_cast<int>(malicious_ratio * fl_devices);

    std::vector<int> indices(fl_devices);
    std::iota(indices.begin(), indices.end(), 0); 
    std::random_shuffle(indices.begin(), indices.end()); 

    for (int i = 0; i < num_malicious; i++) {
        int idx = indices[i];
        if (attack_type == "noByz") {
            no_byz(devices[idx].weights, input_size, output_size);
        } else if (attack_type == "trimAttack") {
            trim_attack(devices[idx].weights, input_size, output_size);
            normalize_weights(devices[idx].weights, input_size, output_size);
        } else if (attack_type == "krumAttack") {
            krum_attack(devices[idx].weights, input_size, output_size);
            normalize_weights(devices[idx].weights, input_size, output_size);
        } else if (attack_type == "fltrustAttack") {
            fltrust_attack(devices[idx].weights, input_size, output_size);
            normalize_weights(devices[idx].weights, input_size, output_size);
        } else if (attack_type == "minMaxAttack") {
            min_max_attack(devices[idx].weights, input_size, output_size);
            normalize_weights(devices[idx].weights, input_size, output_size);
        } else if (attack_type == "minSumAttack") {
            min_sum_attack(devices[idx].weights, input_size, output_size);
            normalize_weights(devices[idx].weights, input_size, output_size);
        } else if (attack_type == "scalingAttack") {
            scaling_attack(devices[idx].weights, input_size, output_size);
            normalize_weights(devices[idx].weights, input_size, output_size); 
        } else {
            std::cerr << "Unknown attack type: " << attack_type << std::endl;
        }
    }
}
