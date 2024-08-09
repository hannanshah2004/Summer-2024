#ifndef AGGREGATIONS_H
#define AGGREGATIONS_H

#include "FCLayer.h"
#include <string>

void simulateAggregation(FCLayer* devices, double** server_weights, double* server_bias, int fl_devices, int input_size, int output_size, const std::string& aggregation_type);

#endif // AGGREGATIONS_H
