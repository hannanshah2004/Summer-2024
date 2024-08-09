// attacks.h
#ifndef ATTACKS_H
#define ATTACKS_H

#include "FCLayer.h"
#include <string>

void simulateAttack(FCLayer* devices, int fl_devices, int input_size, int output_size, const std::string& attack_type);

#endif // ATTACKS_H
