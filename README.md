## Project Overview

In an era of ubiquitous data, ensuring privacy and security in machine learning models is crucial, especially for IoT devices with stringent resource constraints. This project explores various attack and defense mechanisms in federated learning, proposing solutions that are both secure and efficient for resource-constrained environments.

## Key Contributions

- **Aggregation Techniques**: We analyzed several aggregation strategies including Krum, Median, FLTrust, and ROMOA to enhance robustness against adversarial attacks.
- **Malicious Techniques**: This project investigates the impact of different attacks such as Trim Attack, Krum Attack, and Scaling Attack on FL performance.
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
  - **`resultsGraph.py`**: Script to generate graphs from the results of the simulation.

## Highlighted Contributions

### Kavya Kopparapu and Eric Lin

Special acknowledgment to **Kavya Kopparapu** and **Eric Lin** for their significant contributions to the PerCom 2022 demo paper titled "TinyFedTL: Federated Transfer Learning on Ubiquitous Tiny IoT Devices". Their work laid the foundation for implementing federated transfer learning on microcontrollers, addressing the key challenges of privacy, security, and resource constraints in TinyML.
