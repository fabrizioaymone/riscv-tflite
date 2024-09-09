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
- **/cc/**: Source code used for cross-compiling the TFLite Micro API for RISC-V 32-bit implementation and simulation on Gem5.
- **/elf/**: Cross-compiled ELFs for Gem5 simulation.
- **/spike/**: Source code and cross-compiled ELFs for Spike simulation.
- **/results/**: Profiling data and performance analysis using Gem5 simulators (empty, to be filled by running the Makefile).
- **/paper/**: The paper titled *"RISC-V Meets TFLite Micro: Insights from MLCommons Tiny Benchmark"* in PDF format.
- **/scripts/**: Python scripts for configuring RISCV cores for Gem5 simulations.

## Getting Started

### Prerequisites

Before running the code, ensure you have the following installed:

- **Spike Simulator:** The RISC-V ISA simulator with proxy kernel. See installation instructions here for [spike](https://github.com/riscv/riscv-isa-sim) and [pk](https://github.com/riscv-software-src/riscv-pk).
- **RISC-V 32-bit Cross-Compiler:** Required for compiling code targeting the RISC-V 32-bit architecture. Instructions for installation can be found [here](https://github.com/riscv-collab/riscv-gnu-toolchain).
- **Gem5 Simulator:** A modular platform for computer-system architecture research. See installation instructions [here](https://www.gem5.org/documentation/general_docs/building).
- **TFLite Micro:** TensorFlow Lite for Microcontrollers. Refer to the [official TensorFlow documentation](https://www.tensorflow.org/lite/microcontrollers) for setup and installation.

### Usage

In order to produce the .a tflite-micro static library, modify the [Makefile](https://github.com/tensorflow/tflite-micro/blob/89f99a9e7bc0c4db74bd45fdcb8e6e91def406e8/tensorflow/lite/micro/tools/make/Makefile) in tflite-micro github repository by substituting the riscv-gnu-toolchain in the compiler flags and run the Makefile. Beware to first download the external libraries (e.g. flatbuffers, gemmlowp).

To perform Spike and Gem5 use the Makefile and follow the description in the paper.


### Citation

If you use this work in your research, please consider citing the following paper:

```bibtex
@article{aymone2024riscvtflite,
  title={RISC-V Meets TFLite Micro: Insights from MLCommons Tiny Benchmark},
  author={Aymone, Fabrizio Maria},
  url = {https://github.com/github-linguist/linguist](https://github.com/fabrizioaymone/riscv-tflite},
  year={2024}
}

