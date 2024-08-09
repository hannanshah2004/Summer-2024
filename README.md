## Project Overview

In an era of ubiquitous data, ensuring privacy and security in machine learning models is crucial, especially for IoT devices with stringent resource constraints. This project explores various attack and defense mechanisms in federated learning, proposing solutions that are both secure and efficient for resource-constrained environments.

## Key Contributions

- **Aggregation Techniques**:
  - Federated Averaging
  - Krum
  - Median
  - Trim Mean
  - FLTrust
  - ROMOA
- **Malicious Techniques**:
  - No Byzantine Attack
  - Trim Attack
  - Krum Attack
  - FLTrust Attack
  - Min-Max Attack
  - Min-Sum Attack
  - Scaling Attack
- **TinyDistillation**: A custom setup using the CIFAR-10 dataset to simulate federated learning on a resource-constrained device, with a focus on maintaining model integrity through quantization and pruning techniques.

## File Structure

- **`TinyDistillationBasic/allFiles`**:
  - **`aggregations.cc`**: Implementation of various aggregation techniques used in the project.
  - **`aggregations.h`**: Header file defining the structures and functions related to aggregation techniques.
  - **`attacks.cc`**: Implementation of the attack methods analyzed in this project.
  - **`attacks.h`**: Header file defining the structures and functions related to the attack methods.
  - **`FCLayer.h`**: Header file containing the implementation of fully connected layers used in the neural network.
  - **`NeuralNetwork.cpp`**: Contains our fully connected neural network and federated learning implementation.
  - **`simulation.cc`**: The main file for running simulations on federated learning experiments.

## Highlighted Contributions

### Kavya Kopparapu and Eric Lin

Special acknowledgment to **Kavya Kopparapu** and **Eric Lin** for their significant contributions to the PerCom 2022 demo paper titled "TinyFedTL: Federated Transfer Learning on Ubiquitous Tiny IoT Devices". Their work laid the foundation for implementing federated transfer learning on microcontrollers, addressing the key challenges of privacy, security, and resource constraints in TinyML.
