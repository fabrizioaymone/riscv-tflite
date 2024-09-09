# RISC-V Meets TFLite Micro: Insights from MLCommons Tiny Benchmark

This repository contains the materials and code related to the project titled **"RISC-V Meets TFLite Micro: Insights from MLCommons Tiny Benchmark"** for the Advance Computer Architecture course held by Politecnico di Milano.

## Overview

This project explores the deployment of TensorFlow Lite for Microcontrollers (TFLite Micro) on RISC-V architectures. It focuses on benchmarking the MLCommons Tiny models using Spike and Gem5 simulators to evaluate the performance on RISC-V 32 implementations. The study investigates key performance metrics such as CPI and branch mispredictions, particularly comparing in-order and out-of-order core designs.

## Key Topics

- **RISC-V Architecture:** Open-source and customizable for specific machine learning tasks.
- **TFLite Micro:** TensorFlow Lite for Microcontrollers used for edge AI inference.
- **MLCommons Tiny Benchmark:** Standard benchmark for TinyML tasks.
- **Simulators:** Spike and Gem5 used for RISC-V simulation and performance analysis.
- **Core Design:** Performance evaluation of in-order vs. out-of-order core designs.

## Repository Contents

- **/models/**: Contains MLCommons-Tiny benchmark models in TFLite and in their C byte array version.
- **/cc/**: Source code used for cross-compiling the TFLite Micro API for RISC-V 32-bit implementation.
- **/results/**: Profiling data and performance analysis using Gem5 simulators (empty, to be filled by running the Makefile).
- **/paper/**: The paper titled *"RISC-V Meets TFLite Micro: Insights from MLCommons Tiny Benchmark"* in PDF format.
- **/scripts/**: Helper scripts for running benchmarks and simulations on Spike and Gem5.

## Getting Started

### Prerequisites

Before running the code, ensure you have the following installed:

- **Spike Simulator:** The RISC-V ISA simulator. See installation instructions [here](https://github.com/riscv/riscv-isa-sim).
- **Gem5 Simulator:** A modular platform for computer-system architecture research. See installation instructions [here](https://www.gem5.org/documentation/general_docs/building).
- **TFLite Micro:** TensorFlow Lite for Microcontrollers. Refer to the [official TensorFlow documentation](https://www.tensorflow.org/lite/microcontrollers) for setup and installation.

### Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/your-username/riscv-tflite-micro-benchmark.git
