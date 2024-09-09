SHELL := /bin/bash


CXX := riscv32-unknown-elf-g++

GEM5_SRC := /opt/gem5


CFLAGS :=  -O0 -static -Wall -Wextra -D_GEM5_ -I $(GEM5_SRC)/include -I $(GEM5_SRC)/include/gem5/asm/generic 

ASM = $(GEM5_SRC)/util/m5/src/abi/riscv/m5op.S 


TFLITE_DIR := ./tflite-micro

DOWNLOADS_DIR := $(TFLITE_DIR)/tensorflow/lite/micro/tools/make/downloads

MODELS_DIR := models

SPIKE_DIR_C := spike/c

SPIKE_DIR_ELF := spike/elf

C_DIR := cc

ELF_DIR := elf

RESULTS_DIR := results

LIB_DIR := .

LDFLAGS := -L $(LIB_DIR)

LDLIBS := -ltensorflow-microlite 

INCLUDES := \
-I. \
-I$(TFLITE_DIR) \
-I$(DOWNLOADS_DIR)/gemmlowp \
-I$(DOWNLOADS_DIR)/flatbuffers/include \
-I models



all: $(RESULTS_DIR)/o3/adfp32/stats.txt $(RESULTS_DIR)/o3/resnetfp32/stats.txt $(RESULTS_DIR)/o3/kwsfp32/stats.txt $(RESULTS_DIR)/o3/vwwfp32/stats.txt  $(RESULTS_DIR)/io/adfp32/stats.txt $(RESULTS_DIR)/io/resnetfp32/stats.txt $(RESULTS_DIR)/io/kwsfp32/stats.txt $(RESULTS_DIR)/io/vwwfp32/stats.txt  $(RESULTS_DIR)/o3/vwwfp32/stats.txt  $(RESULTS_DIR)/o3_r/adfp32/stats.txt $(RESULTS_DIR)/o3_r/resnetfp32/stats.txt $(RESULTS_DIR)/o3_r/kwsfp32/stats.txt $(RESULTS_DIR)/o3_r/vwwfp32/stats.txt

# SPIKE SIMULATION

PK_DIR := /opt/riscv/riscv32-unknown-elf/bin/pk

spike_sim: simulate_ad simulate_kws simulate_resnet simulate_vww

$(SPIKE_DIR_ELF)/ad_sim: $(SPIKE_DIR_C)/ad_sim.cc $(MODELS_DIR)/adfp32_model.cc $(MODELS_DIR)/adint8_model.cc
	$(CXX) $^ $(INCLUDES) $(LDFLAGS) $(LDLIBS) -o $@

simulate_ad: $(SPIKE_DIR_ELF)/ad_sim
	spike --isa=rv32gc_zicntr $(PK_DIR) $<

$(SPIKE_DIR_ELF)/resnet_sim: $(SPIKE_DIR_C)/resnet_sim.cc $(MODELS_DIR)/resnetfp32_model.cc $(MODELS_DIR)/resnetint8_model.cc
	$(CXX) $^ $(INCLUDES) $(LDFLAGS) $(LIBS) $(LDLIBS) -o $@

simulate_resnet: $(SPIKE_DIR_ELF)/resnet_sim
	spike --isa=rv32gc_zicntr $(PK_DIR) -s $<

$(SPIKE_DIR_ELF)/kws_sim: $(SPIKE_DIR_C)/kws_sim.cc $(MODELS_DIR)/kwsfp32_model.cc $(MODELS_DIR)/kwsint8_model.cc
	$(CXX) $^ $(INCLUDES) $(LDFLAGS) $(LIBS) $(LDLIBS) -o $@

simulate_kws: $(SPIKE_DIR_ELF)/kws_sim 
	spike --isa=rv32gc_zicntr $(PK_DIR) -s $<


$(SPIKE_DIR_ELF)/vww_sim: $(SPIKE_DIR_C)/vww_sim.cc $(MODELS_DIR)/vwwfp32_model.cc $(MODELS_DIR)/vwwint8_model.cc
	$(CXX) $^ $(INCLUDES) $(LDFLAGS) $(LIBS) $(LDLIBS) -o $@

simulate_vww: $(SPIKE_DIR_ELF)/vww_sim 
	spike --isa=rv32gc_zicntr $(PK_DIR) -s $< 




$(SPIKE_DIR_ELF)/test: $(C_DIR)/resnetint8.cc $(MODELS_DIR)/resnetint8_model.cc
	$(CXX) $^ $(INCLUDES) $(LDFLAGS) $(LDLIBS) -o $@

simulate_test: $(SPIKE_DIR_ELF)/test
	spike --isa=rv32gc $(PK_DIR) -s $<



# COMPILATION

#FP32

#RISCV

$(ELF_DIR)/adfp32: $(C_DIR)/adfp32.cc $(MODELS_DIR)/adfp32_model.cc
	$(CXX) $(ASM) $^  $(INCLUDES) $(LDFLAGS) $(LIBS) $(LDLIBS) $(CFLAGS) -o $@

$(ELF_DIR)/resnetfp32: $(C_DIR)/resnetfp32.cc $(MODELS_DIR)/resnetfp32_model.cc
	$(CXX) $(ASM) $^ $(INCLUDES)  $(LDFLAGS) $(LIBS) $(LDLIBS) $(CFLAGS) -o $@

$(ELF_DIR)/kwsfp32: $(C_DIR)/kwsfp32.cc $(MODELS_DIR)/kwsfp32_model.cc
	$(CXX) $(ASM) $^  $(INCLUDES) $(LDFLAGS) $(LIBS) $(LDLIBS) $(CFLAGS) -o $@

$(ELF_DIR)/vwwfp32: $(C_DIR)/vwwfp32.cc $(MODELS_DIR)/vwwfp32_model.cc
	$(CXX) $(ASM) $^ $(INCLUDES) $(LDFLAGS) $(LIBS) $(LDLIBS) $(CFLAGS) -o $@



#RISCV

#INT8

$(ELF_DIR)/adint8: $(C_DIR)/adint8.cc $(MODELS_DIR)/adint8_model.cc
	$(CXX) $(ASM) $^  $(INCLUDES) $(LDFLAGS) $(LIBS) $(LDLIBS) $(CFLAGS) -o $@

$(ELF_DIR)/resnetint8: $(C_DIR)/resnetint8.cc $(MODELS_DIR)/resnetint8_model.cc
	$(CXX) $(ASM) $^ -DTF_LITE_STATIC_MEMORY $(INCLUDES) $(LDFLAGS) $(LIBS) $(LDLIBS) $(CFLAGS) -o $@

$(ELF_DIR)/kwsint8: $(C_DIR)/kwsint8.cc $(MODELS_DIR)/kwsint8_model.cc
	$(CXX) $(ASM) $^ $(INCLUDES) $(LDFLAGS) $(LIBS) $(LDLIBS) $(CFLAGS) -o $@

$(ELF_DIR)/vwwint8: $(C_DIR)/vwwint8.cc $(MODELS_DIR)/vwwint8_model.cc
	$(CXX) $(ASM) $^ $(INCLUDES) $(LDFLAGS) $(LIBS) $(LDLIBS) $(CFLAGS) -o $@



# GEM5 SIMULATION

#FP32

#RISCV
setup:
	source setup.sh


# optional flags for pipeline viewing and tracing --debug-flags=O3PipeView,O3CPUAll --debug-file=adfp32_trace.out

$(RESULTS_DIR)/o3/adfp32/stats.txt: $(ELF_DIR)/adfp32
	/opt/gem5/build/RISCV/gem5.opt   --debug-flags=O3PipeView --debug-file=trace.out --outdir=results/o3/adfp32 --verbose ./script/riscv_o3_custom.py --caches -c $(ELF_DIR)/adfp32 --directory=results/o3/adfp32 --errout=adfp32.err 

$(RESULTS_DIR)/o3/resnetfp32/stats.txt: $(ELF_DIR)/resnetfp32
	/opt/gem5/build/RISCV/gem5.opt   --outdir=results/o3/resnetfp32 --verbose ./script/riscv_o3_custom.py --caches -c $(ELF_DIR)/resnetfp32 --directory=results/o3/resnetfp32 --errout=resnetfp32.err

$(RESULTS_DIR)/o3/kwsfp32/stats.txt: $(ELF_DIR)/kwsfp32
	/opt/gem5/build/RISCV/gem5.opt   --outdir=results/o3/kwsfp32 --verbose ./script/riscv_o3_custom.py --caches -c $(ELF_DIR)/kwsfp32 --directory=results/o3/kwsfp32 --errout=kwsfp32.err

$(RESULTS_DIR)/o3/vwwfp32/stats.txt: $(ELF_DIR)/vwwfp32
	/opt/gem5/build/RISCV/gem5.opt   --outdir=results/o3/vwwfp32 --verbose ./script/riscv_o3_custom.py --caches -c $(ELF_DIR)/vwwfp32 --directory=results/o3/vwwfp32 --errout=vwwfp32.err

# IN-ORDER

$(RESULTS_DIR)/io/adfp32/stats.txt: $(ELF_DIR)/adfp32
	/opt/gem5/build/RISCV/gem5.opt   -debug-flags=Minor,Exec --debug-file=trace.out --outdir=results/io/adfp32 --verbose ./script/riscv_minor_custom.py --caches -c $(ELF_DIR)/adfp32 --directory=results/io/adfp32 --errout=adfp32.err 

$(RESULTS_DIR)/io/resnetfp32/stats.txt: $(ELF_DIR)/resnetfp32
	/opt/gem5/build/RISCV/gem5.opt   --outdir=results/io/resnetfp32 --verbose ./script/riscv_minor_custom.py --caches -c $(ELF_DIR)/resnetfp32 --directory=results/io/resnetfp32 --errout=resnetfp32.err

$(RESULTS_DIR)/io/kwsfp32/stats.txt: $(ELF_DIR)/kwsfp32
	/opt/gem5/build/RISCV/gem5.opt   --outdir=results/io/kwsfp32 --verbose ./script/riscv_minor_custom.py --caches -c $(ELF_DIR)/kwsfp32 --directory=results/io/kwsfp32 --errout=kwsfp32.err

$(RESULTS_DIR)/io/vwwfp32/stats.txt: $(ELF_DIR)/vwwfp32
	/opt/gem5/build/RISCV/gem5.opt   --outdir=results/io/vwwfp32 --verbose ./script/riscv_minor_custom.py --caches -c $(ELF_DIR)/vwwfp32 --directory=results/io/vwwfp32 --errout=vwwfp32.err

#rename

$(RESULTS_DIR)/o3_r/adfp32/stats.txt: $(ELF_DIR)/adfp32
	/opt/gem5/build/RISCV/gem5.opt   --debug-flags=O3PipeView --debug-file=trace.out --outdir=results/o3_r/adfp32 --verbose ./script/riscv_o3_custom_rename.py --caches -c $(ELF_DIR)/adfp32 --directory=results/o3_r/adfp32 --errout=adfp32.err 

$(RESULTS_DIR)/o3_r/resnetfp32/stats.txt: $(ELF_DIR)/resnetfp32
	/opt/gem5/build/RISCV/gem5.opt   --outdir=results/o3_r/resnetfp32 --verbose ./script/riscv_o3_custom_rename.py --caches -c $(ELF_DIR)/resnetfp32 --directory=results/o3_r/resnetfp32 --errout=resnetfp32.err

$(RESULTS_DIR)/o3_r/kwsfp32/stats.txt: $(ELF_DIR)/kwsfp32
	/opt/gem5/build/RISCV/gem5.opt   --outdir=results/o3_r/kwsfp32 --verbose ./script/riscv_o3_custom_rename.py --caches -c $(ELF_DIR)/kwsfp32 --directory=results/o3_r/kwsfp32 --errout=kwsfp32.err

$(RESULTS_DIR)/o3_r/vwwfp32/stats.txt: $(ELF_DIR)/vwwfp32
	/opt/gem5/build/RISCV/gem5.opt   --outdir=results/o3_r/vwwfp32 --verbose ./script/riscv_o3_custom_rename.py --caches -c $(ELF_DIR)/vwwfp32 --directory=results/o3_r/vwwfp32 --errout=vwwfp32.err



.PHONY: clean
clean:
	find results -mindepth 2  -exec rm -r {} \;
	find elf -mindepth 1  -exec rm -r {} \;

