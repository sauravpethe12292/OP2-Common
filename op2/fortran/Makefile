#
# The following environment variables are required:
#
# CUDA_INSTALL_PATH - CUDA base directory
# OP2_COMPILER      - Compiler toolchain: gnu (gcc, gfortran) or pgi (icc, pgfortran)
# OP2_INSTALL_PATH  - OP2 base directory
#
# Note: the CUDA library can only be built with the PGI toolchain
OP2 = $(OP2_INSTALL_PATH)
C_OP2 = $(OP2)/c
C_OP2_INC = $(C_OP2)/include
C_OP2_SRC = $(C_OP2)/src

F_OP2 = $(OP2)/fortran
F_OP2_INC = $(F_OP2)/include
F_OP2_SRC = $(F_OP2)/src
F_OP2_OBJ = $(F_OP2)/obj
F_OP2_LIB = $(F_OP2)/lib
F_OP2_MOD = $(F_OP2)/mod

PARMETIS_INCLUDE_PATH ?= $(PARMETIS_INSTALL_PATH)
PARMETIS_LIB_PATH ?= $(PARMETIS_INSTALL_PATH)

PARMETIS_VER?=4
ifeq ($(PARMETIS_VER),4)
  PARMETIS_INC = -I$(PARMETIS_INCLUDE_PATH)/include -DHAVE_PARMETIS -DPARMETIS_VER_4
  PARMETIS_LIB = -L$(PARMETIS_LIB_PATH)/lib -lparmetis -lmetis
else
  PARMETIS_INC = -I$(PARMETIS_INCLUDE_PATH)/ -DHAVE_PARMETIS
  PARMETIS_LIB = -L$(PARMETIS_LIB_PATH)/ -lparmetis -lmetis
endif

PTSCOTCH_INCLUDE_PATH ?= $(PTSCOTCH_INSTALL_PATH)/include
PTSCOTCH_LIB_PATH ?= $(PTSCOTCH_INSTALL_PATH)/lib
PTSCOTCH_INC = -I$(PTSCOTCH_INCLUDE_PATH) -DHAVE_PTSCOTCH
PTSCOTCH_LIB = -L$(PTSCOTCH_LIB_PATH) -lptscotch -lptscotcherr

HDF5_INCLUDE_PATH ?= $(HDF5_INSTALL_PATH)/include
HDF5_INCLUDE =  -I$(HDF5_INCLUDE_PATH)
HDF5_LIB_PATH ?= $(HDF5_INSTALL_PATH)/lib
HDF5_LIB =  -L$(HDF5_LIB_PATH) -lhdf5

CUDA_INC = $(CUDA_INSTALL_PATH)/include

MPI_INC = $(MPI_INSTALL_PATH)/include
MPI_LIB = $(MPI_INSTALL_PATH)/lib

#
# compiler settings
#
NVCCFLAGS = -O3 -arch=sm_20 -m64 -Xptxas -dlcm=cg -Xptxas=-v -use_fast_math
# Default to PGI compiler
#OP2_COMPILER ?= pgi

# Common definitions
# Uncomment -g to get debugging info on board
# DEBUG_FLAGS = -O2

#  -Werror

# All compilers define CC and FC plus compiler-specific macros
ifeq ($(OP2_COMPILER),gnu)
  F_INC_MOD       = $(F_OP2_MOD)/gnu
  CC              = gcc
  CXX             = g++
  CFLAGS          = -std=c99 -fPIC -Wall -pedantic -pipe $(DEBUG_FLAGS)
  FC              = gfortran
  FFLAGS          = -J$(F_INC_MOD) -fPIC -Wall -pedantic -pipe $(DEBUG_FLAGS)
  MPICC           = mpicc
  MPICXX          = mpicxx
  MPIF90          = mpif90
  NO_MPI_MACRO    = -DNO_MPI
  ALLTARGETS      = core hdf5 f_seq f_openmp f_mpi_seq
else
ifeq ($(OP2_COMPILER),pgi)
  F_INC_MOD       = $(F_OP2_MOD)/pgi
  CC              = nvcc
  CXX             = pgCC
  FC              = pgfortran
  FFLAGS          = -module $(F_INC_MOD) $(DEBUG_FLAGS)
  NVCC            = nvcc
  PGCC            = pgcc
  PGCFLAGS        = -O2 -Xcompiler="-std=c99 -Wall -fPIC -pedantic -pipe" $(DEBUG_FLAGS)
  PGFC            = pgfortran
  PGFFLAGS        = -module $(F_INC_MOD) -fast -O2 -Mcuda=cuda5.0,cc35 -Minform=inform -DOP2_WITH_CUDAFOR $(DEBUG_FLAGS) -Mvect –Mvect=cachesize:12288
  CFLAGS          = $(PGCFLAGS)
  MPICC           = $(CC) -I$(MPI_INC) -L$(MPI_LIB)
  MPICXX          = $(CXX) -I$(MPI_INC) -L$(MPI_LIB)
  MPIF90          = $(FC) -I$(MPI_INC) -L$(MPI_LIB)
  NO_MPI_MACRO    = -DNO_MPI
  ALLTARGETS      = clean core hdf5 f_seq f_openmp f_mpi_seq f_cuda f_mpi_cuda f_mpi_cuda
else
ifeq ($(OP2_COMPILER),intel)
  F_INC_MOD       = $(F_OP2_MOD)/intel
  OPT             = -O3 -xAVX -vec-report
  CC              = icc $(OPT)
  CXX             = icpc $(OPT)
  CFLAGS          = -std=c99 -Wall -Werror $(DEBUG_FLAGS) -wd981
  FC              = ifort $(OPT)
  FFLAGS          = -module $(F_INC_MOD) $(DEBUG_FLAGS)
  MPICC           = $(MPI_INSTALL_PATH)/bin/mpicc $(OPT)
  MPICXX          = $(MPI_INSTALL_PATH)/bin/mpiCC $(OPT)
  MPIF90          = $(MPI_INSTALL_PATH)/bin/mpif90 $(OPT)
  NO_MPI_MACRO    = -D NO_MPI
  ALLTARGETS      = clean core hdf5 f_seq f_openmp f_mpi_seq
else
ifeq ($(OP2_COMPILER),cray)
  F_INC_MOD       = $(F_OP2_MOD)/cray
  OPT             = -O2
  CC              = cc -O2
  CXX             = CC -O2
  CFLAGS          = #-std=c99 -Wall
  FC              = ftn
  FFLAGS          = -em -J $(F_INC_MOD)
  MPICC           = $(CC)
  MPICXX          = $(CXX)
  MPIF90          = $(FC)
  NO_MPI_MACRO    = -D NO_MPI
  ALLTARGETS      = clean core hdf5 f_seq f_openmp f_mpi_seq
else
print:
	@echo "unrecognised value for OP2_COMPILER"
endif
endif
endif
endif


.PHONY: clean directories

all: $(ALLTARGETS)

directories:
	mkdir -p $(F_OP2_LIB)
	mkdir -p $(F_OP2_MOD) $(F_INC_MOD)
	mkdir -p $(F_OP2_OBJ)

core: 	directories $(C_OP2_INC)/op_lib_core.h $(C_OP2_SRC)/core/op_lib_core.c
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/core/op_lib_core.c -o $(F_OP2_OBJ)/op_lib_core.o
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/externlib/op_util.c -o $(F_OP2_OBJ)/op_util.o

hdf5:   directories $(C_OP2_INC)/op_hdf5.h $(C_OP2_SRC)/externlib/op_hdf5.c
	$(CC) $(CFLAGS) $(HDF5_INCLUDE) -I$(MPI_INC) -I$(C_OP2_INC) -c $(C_OP2_SRC)/externlib/op_hdf5.c -o $(F_OP2_OBJ)/op_hdf5.o
	$(FC) $(FFLAGS) -c $(F_OP2_SRC)/op2_for_declarations.F90 -o $(F_OP2_OBJ)/op2_for_declarations.o
	$(FC) $(FFLAGS) -c $(F_OP2_SRC)/op2_for_hdf5_declarations.F90 -o $(F_OP2_OBJ)/op2_for_hdf5_declarations.o
	ar -r $(F_OP2_LIB)/libop2_hdf5.a $(F_OP2_OBJ)/op_hdf5.o $(F_OP2_OBJ)/op_util.o $(F_OP2_OBJ)/op2_for_hdf5_declarations.o

f_seq:	directories core $(C_OP2_SRC)/sequential/op_seq.c \
	$(C_OP2_SRC)/core/op_dummy_singlenode.c \
	$(F_OP2_SRC)/op_dummy_wrappers.c \
	$(F_OP2_SRC)/op2_for_C_wrappers.c \
	$(F_OP2_SRC)/op2_for_declarations.F90 \
	$(F_OP2_SRC)/op2_for_reference.F90 \
	$(F_OP2_SRC)/op2_C_reference.c \
	$(F_OP2_SRC)/op2_for_rt_support.F90
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/sequential/op_seq.c -o $(F_OP2_OBJ)/op_seq.o
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/core/op_dummy_singlenode.c -o $(F_OP2_OBJ)/op_dummy_singlenode.o
	$(CC) $(CFLAGS) $(NO_MPI_MACRO) -I $(F_OP2_INC) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op_dummy_wrappers.c -o $(F_OP2_OBJ)/op_dummy_wrappers.o
	$(CC) $(CFLAGS) $(NO_MPI_MACRO) -I$(C_OP2_INC) -I$(MPI_INC) -c $(F_OP2_SRC)/op2_for_C_wrappers.c -o $(F_OP2_OBJ)/op2_for_C_wrappers.o
	$(FC) $(FFLAGS) -c $(F_OP2_SRC)/op2_for_declarations.F90 -o $(F_OP2_OBJ)/op2_for_declarations.o
	$(shell cp $(F_OP2_SRC)/op2_for_reference.F90 $(F_OP2_SRC)/op2_for_reference.c)
	$(shell cpp $(F_OP2_SRC)/op2_for_reference.c | sed s/##//g | sed s/\"@\"//g | tr "@" "\\n" > $(F_OP2_SRC)/op2_cpp_reference.F90)
	$(FC) $(FFLAGS) -c $(F_OP2_SRC)/op2_cpp_reference.F90 -o $(F_OP2_OBJ)/op2_for_reference.o
	$(CC) $(CFLAGS) $(NO_MPI_MACRO) -I$(C_OP2_INC) -I$(MPI_INC) -c $(F_OP2_SRC)/op2_C_reference.c -o $(F_OP2_OBJ)/op2_C_reference.o
	$(FC) $(FFLAGS) -c $(F_OP2_SRC)/op2_for_rt_support.F90 -o $(F_OP2_OBJ)/op2_for_rt_support.o
	ar -r $(F_OP2_LIB)/libop2_for_seq.a \
	$(F_OP2_OBJ)/op_lib_core.o \
	$(F_OP2_OBJ)/op_util.o \
	$(F_OP2_OBJ)/op_seq.o \
	$(F_OP2_OBJ)/op_dummy_singlenode.o \
	$(F_OP2_OBJ)/op_dummy_wrappers.o \
	$(F_OP2_OBJ)/op2_for_reference.o \
	$(F_OP2_OBJ)/op2_for_C_wrappers.o \
	$(F_OP2_OBJ)/op2_for_declarations.o \
	$(F_OP2_OBJ)/op2_C_reference.o \
	$(F_OP2_OBJ)/op2_for_rt_support.o

f_openmp: directories core $(C_OP2_SRC)/openmp/op_openmp_decl.c \
	$(F_OP2_SRC)/op2_for_declarations.F90 \
	$(C_OP2_INC)/op_rt_support.h $(C_OP2_SRC)/core/op_rt_support.c \
	$(F_OP2_SRC)/op2_for_rt_wrappers.c \
	$(F_OP2_SRC)/op2_for_rt_support.F90 \
	$(F_OP2_SRC)/op_dummy_wrappers.c \
	$(C_OP2_SRC)/core/op_dummy_singlenode.c \
	$(C_OP2_SRC)/externlib/op_util.c \
	$(F_OP2_SRC)/op2_for_reference.F90 \
	$(F_OP2_SRC)/op2_C_reference.c
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/core/op_lib_core.c -o $(F_OP2_OBJ)/op_lib_core.o
	$(CC) $(CFLAGS) $(NO_MPI_MACRO) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op2_for_C_wrappers.c -o $(F_OP2_OBJ)/op2_for_C_wrappers.o
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/core/op_rt_support.c -o $(F_OP2_OBJ)/op_rt_support.o
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/openmp/op_openmp_decl.c -o $(F_OP2_OBJ)/op_openmp_decl.o
	$(FC) $(FFLAGS) -c $(F_OP2_SRC)/op2_for_declarations.F90 -o $(F_OP2_OBJ)/op2_for_declarations.o
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op2_for_rt_wrappers.c -o $(F_OP2_OBJ)/op2_for_rt_wrappers.o
	$(FC) $(FFLAGS) -c $(F_OP2_SRC)/op2_for_rt_support.F90 -o $(F_OP2_OBJ)/op2_for_rt_support.o
	$(CC) $(CFLAGS) $(HDF5_INCLUDE) -I$(MPI_INC) -I$(C_OP2_INC) -c $(C_OP2_SRC)/externlib/op_hdf5.c -o $(F_OP2_OBJ)/op_hdf5.o
	$(CC) $(CFLAGS) -I $(F_OP2_INC) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op_dummy_wrappers.c -o $(F_OP2_OBJ)/op_dummy_wrappers.o
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/core/op_dummy_singlenode.c -o $(F_OP2_OBJ)/op_dummy_singlenode.o
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/externlib/op_util.c -o $(F_OP2_OBJ)/op_util.o
	$(CC) $(CFLAGS) $(NO_MPI_MACRO) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op2_C_reference.c -o $(F_OP2_OBJ)/op2_C_reference.o
	$(shell cp $(F_OP2_SRC)/op2_for_reference.F90 $(F_OP2_SRC)/op2_for_reference.c)
	$(shell cpp $(F_OP2_SRC)/op2_for_reference.c | sed s/##//g | sed s/\"@\"//g | tr "@" "\\n" > $(F_OP2_SRC)/op2_cpp_reference.F90)
	$(FC) -c $(FFLAGS) $(F_OP2_SRC)/op2_cpp_reference.F90 -o $(F_OP2_OBJ)/op2_for_reference.o
	ar -r $(F_OP2_LIB)/libop2_for_openmp.a \
	$(F_OP2_OBJ)/op2_for_C_wrappers.o \
	$(F_OP2_OBJ)/op_lib_core.o \
	$(F_OP2_OBJ)/op_rt_support.o \
	$(F_OP2_OBJ)/op_openmp_decl.o \
	$(F_OP2_OBJ)/op2_for_declarations.o \
	$(F_OP2_OBJ)/op2_for_rt_wrappers.o \
	$(F_OP2_OBJ)/op2_for_rt_support.o \
	$(F_OP2_OBJ)/op_dummy_wrappers.o \
	$(F_OP2_OBJ)/op_dummy_singlenode.o \
	$(F_OP2_OBJ)/op_util.o \
	$(F_OP2_OBJ)/op2_for_reference.o \
	$(F_OP2_OBJ)/op2_C_reference.o

f_cuda: directories core $(C_OP2_SRC)/core/op_lib_core.c \
	$(C_OP2_INC)/op_cuda_rt_support.h $(C_OP2_SRC)/cuda/op_cuda_rt_support.c \
	$(C_OP2_SRC)/cuda/op_cuda_decl.c \
	$(F_OP2_SRC)/op2_for_rt_wrappers.c \
	$(F_OP2_SRC)/op2_for_rt_support.F90 \
	$(C_OP2_INC)/op_rt_support.h $(C_OP2_SRC)/core/op_rt_support.c \
	$(F_OP2_SRC)/op2_for_rt_wrappers_cuda.c \
	$(F_OP2_SRC)/op2_hdf5_dummy.c directories
	$(NVCC) $(PGCFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/core/op_lib_core.c -o $(F_OP2_OBJ)/op_lib_core.o
	$(NVCC) $(PGCFLAGS) -I$(C_OP2_INC) -I$(CUDA_INC) -c $(C_OP2_SRC)/cuda/op_cuda_decl.c -o $(F_OP2_OBJ)/op_cuda_decl.o
	$(NVCC) $(PGCFLAGS) -I$(C_OP2_INC) -I$(CUDA_INC) -c $(C_OP2_SRC)/cuda/op_cuda_rt_support.c -o $(F_OP2_OBJ)/op_cuda_rt_support.o
	$(NVCC) $(PGCFLAGS) $(NO_MPI_MACRO) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op2_for_C_wrappers.c -o $(F_OP2_OBJ)/op2_for_C_wrappers.o
	$(PGFC) $(PGFFLAGS) -c $(F_OP2_SRC)/op2_for_declarations.F90 -o $(F_OP2_OBJ)/op2_for_declarations.o
	$(PGFC) $(PGFFLAGS) -c $(F_OP2_SRC)/op2_for_rt_support.F90 -o $(F_OP2_OBJ)/op2_for_rt_support.o
	$(PGFC) $(PGFFLAGS) -c $(F_OP2_SRC)/cudaConfigurationParams.F90 -o $(F_OP2_OBJ)/cudaConfigurationParams.o
	$(NVCC) $(PGCFLAGS) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op2_for_rt_wrappers.c -o $(F_OP2_OBJ)/op2_for_rt_wrappers.o
	$(NVCC) $(PGCFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/core/op_rt_support.c -o $(F_OP2_OBJ)/op_rt_support.o
	$(NVCC) $(PGCFLAGS) -I$(C_OP2_INC) -I$(F_OP2_SRC)/ -I$(CUDA_INC) -c $(F_OP2_SRC)/op2_for_rt_wrappers_cuda.c -o $(F_OP2_OBJ)/op2_for_rt_wrappers_cuda.o
	ar -r $(F_OP2_LIB)/libop2_for_cuda.a \
	$(F_OP2_OBJ)/op_lib_core.o \
	$(F_OP2_OBJ)/op_rt_support.o \
	$(F_OP2_OBJ)/op_cuda_decl.o \
	$(F_OP2_OBJ)/op_cuda_rt_support.o \
	$(F_OP2_OBJ)/op2_for_C_wrappers.o \
	$(F_OP2_OBJ)/op2_for_declarations.o \
	$(F_OP2_OBJ)/op2_C_reference.o \
	$(F_OP2_OBJ)/op2_for_reference.o \
	$(F_OP2_OBJ)/op2_for_rt_support.o \
	$(F_OP2_OBJ)/op2_for_C_wrappers.o \
	$(F_OP2_OBJ)/op2_for_rt_wrappers.o \
	$(F_OP2_OBJ)/op2_for_rt_wrappers_cuda.o \
	$(F_OP2_OBJ)/cudaConfigurationParams.o


f_mpi_seq: directories core \
	$(C_OP2_INC)/op_rt_support.h $(C_OP2_SRC)/core/op_rt_support.c \
	$(F_OP2_SRC)/op2_for_C_wrappers.c \
	$(C_OP2_SRC)/externlib/op_util.c \
	$(F_OP2_SRC)/op2_for_declarations.F90 \
	$(F_OP2_SRC)/op2_for_hdf5_declarations.F90 \
	$(F_OP2_SRC)/op2_for_rt_support.F90 \
	$(F_OP2_SRC)/op2_for_rt_wrappers.c \
	$(F_OP2_SRC)/op2_for_reference.F90 \
	$(F_OP2_SRC)/op_dummy_wrappers.c \
	$(F_OP2_SRC)/op2_C_reference.c \
	$(C_OP2_INC)/op_lib_mpi.h  $(C_OP2_INC)/op_mpi_core.h \
	$(C_OP2_SRC)/mpi/op_mpi_core.c $(C_OP2_SRC)/mpi/op_mpi_decl.c \
	$(C_OP2_SRC)/mpi/op_mpi_part_core.c $(C_OP2_SRC)/mpi/op_mpi_rt_support.c \
	$(C_OP2_SRC)/mpi/op_mpi_util.cpp \
	$(C_OP2_SRC)/externlib/op_renumber.cpp \
	$(C_OP2_SRC)/mpi/op_mpi_hdf5.c
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/core/op_lib_core.c -o $(F_OP2_OBJ)/op_lib_core.o
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/core/op_rt_support.c -o $(F_OP2_OBJ)/op_rt_support.o
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/externlib/op_util.c -o $(F_OP2_OBJ)/op_util.o
	$(CC) $(CFLAGS) -I$(F_OP2_INC) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op_dummy_wrappers.c -o $(F_OP2_OBJ)/op_dummy_wrappers.o
	$(CC) $(CFLAGS) -I$(MPI_INC) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op2_for_C_wrappers.c -o $(F_OP2_OBJ)/op2_for_C_wrappers.o
	$(FC) $(FFLAGS) -c $(F_OP2_SRC)/op2_for_declarations.F90 -o $(F_OP2_OBJ)/op2_for_declarations.o
	$(FC) $(FFLAGS) -c $(F_OP2_SRC)/op2_for_hdf5_declarations.F90 -o $(F_OP2_OBJ)/op2_for_hdf5_declarations.o
	$(FC) $(FFLAGS) -c $(F_OP2_SRC)/op2_for_rt_support.F90 -o $(F_OP2_OBJ)/op2_for_rt_support.o
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op2_for_rt_wrappers.c -o $(F_OP2_OBJ)/op2_for_rt_wrappers.o
	$(shell cp $(F_OP2_SRC)/op2_for_reference.F90 $(F_OP2_SRC)/op2_for_reference.c)
	$(shell cpp $(F_OP2_SRC)/op2_for_reference.c | sed s/##//g | sed s/\"@\"//g | tr "@" "\\n" > $(F_OP2_SRC)/op2_cpp_reference.F90)
	$(FC) -c $(FFLAGS) $(F_OP2_SRC)/op2_cpp_reference.F90 -o $(F_OP2_OBJ)/op2_for_reference.o
	$(CC) $(CFLAGS) -I$(C_OP2_INC) -I$(MPI_INC) -c $(F_OP2_SRC)/op2_C_reference.c -o $(F_OP2_OBJ)/op2_C_reference.o
	$(MPICC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/mpi/op_mpi_core.c -o $(F_OP2_OBJ)/op_mpi_core.o
	$(MPICC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/mpi/op_mpi_decl.c -o $(F_OP2_OBJ)/op_mpi_decl.o
	$(MPICC) $(CFLAGS) -I$(C_OP2_INC) $(PARMETIS_INC) $(PARMETIS_LIB) $(PTSCOTCH_INC) $(PTSCOTCH_LIB) \
	-c $(C_OP2_SRC)/mpi/op_mpi_part_core.c -o $(F_OP2_OBJ)/op_mpi_part_core.o
	$(MPICC) $(CFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/mpi/op_mpi_rt_support.c -o $(F_OP2_OBJ)/op_mpi_rt_support.o
	$(CC) $(CFLAGS) -I$(MPI_INC) $(HDF5_INCLUDE) -I$(C_OP2_INC)  -I$(CUDA_INC) -c $(C_OP2_SRC)/mpi/op_mpi_hdf5.c -o $(F_OP2_OBJ)/op_mpi_hdf5.o
	$(MPICXX) $(CFLAGS) $(HDF5_INCLUDE) -I$(C_OP2_INC) -I$(CUDA_INC) -c $(C_OP2_SRC)/mpi/op_mpi_util.cpp -o $(F_OP2_OBJ)/op_mpi_util.o
	$(MPICXX) $(CFLAGS) $(HDF5_INCLUDE) -I$(C_OP2_INC) -I$(CUDA_INC) $(PARMETIS_INC) $(PARMETIS_LIB) $(PTSCOTCH_INC) $(PTSCOTCH_LIB) -c $(C_OP2_SRC)/externlib/op_renumber.cpp -o $(F_OP2_OBJ)/op_renumber.o
	ar -r $(F_OP2_LIB)/libop2_for_mpi.a \
	$(F_OP2_OBJ)/op_lib_core.o \
	$(F_OP2_OBJ)/op_rt_support.o \
	$(F_OP2_OBJ)/op_util.o \
	$(F_OP2_OBJ)/op_dummy_wrappers.o \
	$(F_OP2_OBJ)/op2_for_C_wrappers.o \
	$(F_OP2_OBJ)/op2_for_declarations.o \
	$(F_OP2_OBJ)/op2_for_hdf5_declarations.o \
	$(F_OP2_OBJ)/op2_for_rt_support.o \
	$(F_OP2_OBJ)/op2_for_rt_wrappers.o \
	$(F_OP2_OBJ)/op_mpi_core.o \
	$(F_OP2_OBJ)/op_mpi_util.o \
	$(F_OP2_OBJ)/op_renumber.o \
	$(F_OP2_OBJ)/op_mpi_decl.o \
	$(F_OP2_OBJ)/op_mpi_part_core.o \
	$(F_OP2_OBJ)/op_mpi_rt_support.o \
	$(F_OP2_OBJ)/op2_C_reference.o \
	$(F_OP2_OBJ)/op2_for_reference.o \
	$(F_OP2_OBJ)/op_mpi_hdf5.o

f_mpi_cuda: directories core \
	$(C_OP2_INC)/op_lib_core.h $(C_OP2_SRC)/core/op_lib_core.c \
	$(C_OP2_INC)/op_rt_support.h $(C_OP2_SRC)/core/op_rt_support.c \
	$(F_OP2_SRC)/op2_for_C_wrappers.c \
	$(C_OP2_SRC)/externlib/op_util.c \
	$(F_OP2_SRC)/op2_for_declarations.F90 \
	$(F_OP2_SRC)/op2_for_hdf5_declarations.F90 \
	$(F_OP2_SRC)/op2_for_rt_support.F90 \
	$(F_OP2_SRC)/op2_for_rt_wrappers.c \
	$(F_OP2_SRC)/op2_for_rt_wrappers_cuda.c \
	$(F_OP2_SRC)/op2_for_reference.F90 \
	$(F_OP2_SRC)/op2_C_reference.c \
	$(C_OP2_INC)/op_lib_mpi.h $(C_OP2_INC)/op_mpi_core.h \
	$(C_OP2_SRC)/mpi/op_mpi_core.c $(C_OP2_SRC)/mpi/op_mpi_cuda_decl.c \
	$(C_OP2_SRC)/mpi/op_mpi_part_core.c $(C_OP2_SRC)/mpi/op_mpi_cuda_rt_support.c \
	$(C_OP2_SRC)/mpi/op_mpi_cuda_kernels.cu \
	$(C_OP2_SRC)/mpi/op_mpi_util.cpp \
	$(C_OP2_SRC)/externlib/op_renumber.cpp \
	$(C_OP2_SRC)/mpi/op_mpi_hdf5.c
	$(NVCC) $(PGCFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/core/op_lib_core.c -o $(F_OP2_OBJ)/op_lib_core.o
	$(NVCC) $(PGCFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/core/op_rt_support.c -o $(F_OP2_OBJ)/op_rt_support.o
	$(NVCC) $(PGCFLAGS) -I$(C_OP2_INC) -c $(C_OP2_SRC)/externlib/op_util.c -o $(F_OP2_OBJ)/op_util.o
	$(NVCC) $(PGCFLAGS) -I$(MPI_INC) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op2_for_C_wrappers.c -o $(F_OP2_OBJ)/op2_for_C_wrappers.o
	$(PGFC) $(PGFFLAGS) -c $(F_OP2_SRC)/op2_for_declarations.F90 -o $(F_OP2_OBJ)/op2_for_declarations.o
	$(FC) $(FFLAGS) -c $(F_OP2_SRC)/op2_for_hdf5_declarations.F90 -o $(F_OP2_OBJ)/op2_for_hdf5_declarations.o
	$(PGFC) $(PGFFLAGS) -c $(F_OP2_SRC)/op2_for_rt_support.F90 -o $(F_OP2_OBJ)/op2_for_rt_support.o
	$(NVCC) $(PGCFLAGS) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op2_for_rt_wrappers.c -o $(F_OP2_OBJ)/op2_for_rt_wrappers.o
	$(NVCC) $(PGCFLAGS) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op2_for_rt_wrappers_cuda.c -o $(F_OP2_OBJ)/op2_for_rt_wrappers_cuda.o
	$(shell cp $(F_OP2_SRC)/op2_for_reference.F90 $(F_OP2_SRC)/op2_for_reference.c)
	$(shell cpp $(F_OP2_SRC)/op2_for_reference.c | sed s/##//g | sed s/\"@\"//g | tr "@" "\\n" > $(F_OP2_SRC)/op2_cpp_reference.F90)
	$(PGFC) -c $(PGFFLAGS) $(F_OP2_SRC)/op2_cpp_reference.F90 -o $(F_OP2_OBJ)/op2_for_reference.o
	$(NVCC) $(PGCFLAGS) -I$(MPI_INC) -I$(C_OP2_INC) -c $(F_OP2_SRC)/op2_C_reference.c -o $(F_OP2_OBJ)/op2_C_reference.o
	$(NVCC) $(PGCFLAGS) -I$(MPI_INC) -I$(C_OP2_INC) -c $(C_OP2_SRC)/mpi/op_mpi_core.c -o $(F_OP2_OBJ)/op_mpi_core.o
	$(NVCC) $(PGCFLAGS) -I$(MPI_INC) -I$(C_OP2_INC) -c $(C_OP2_SRC)/mpi/op_mpi_cuda_decl.c -o $(F_OP2_OBJ)/op_mpi_cuda_decl.o
	$(NVCC) $(PGCFLAGS) -I$(MPI_INC) -I$(C_OP2_INC) -DOPMPI -c $(C_OP2_SRC)/cuda/op_cuda_rt_support.c -o $(F_OP2_OBJ)/op_cuda_rt_support.o
	$(NVCC) $(PGCFLAGS) -I$(MPI_INC) -I$(C_OP2_INC) $(PTSCOTCH_INC) $(PTSCOTCH_LIB) $(PARMETIS_INC) $(PARMETIS_LIB) -c $(C_OP2_SRC)/mpi/op_mpi_part_core.c -o $(F_OP2_OBJ)/op_mpi_part_core.o
	$(NVCC) $(PGCFLAGS) -I$(MPI_INC) -I$(C_OP2_INC) -c $(C_OP2_SRC)/mpi/op_mpi_cuda_rt_support.c -o $(F_OP2_OBJ)/op_mpi_cuda_rt_support.o
	$(NVCC) $(NVCCFLAGS) -I$(MPI_INC) -I$(C_OP2_INC) -c $(C_OP2_SRC)/mpi/op_mpi_cuda_kernels.cu -o $(F_OP2_OBJ)/op_mpi_cuda_kernels.o
	$(NVCC) $(PGCFLAGS) -I$(MPI_INC) $(HDF5_INCLUDE) -I$(C_OP2_INC) -I$(CUDA_INC) -c $(C_OP2_SRC)/mpi/op_mpi_hdf5.c -o $(F_OP2_OBJ)/op_mpi_hdf5.o
	$(MPICXX) $(CFLAGS) $(HDF5_INCLUDE) -I$(C_OP2_INC) -I$(CUDA_INC) -c $(C_OP2_SRC)/mpi/op_mpi_util.cpp -o $(F_OP2_OBJ)/op_mpi_util.o
	$(MPICXX) $(CFLAGS) $(HDF5_INCLUDE) -I$(C_OP2_INC) -I$(CUDA_INC) $(PTSCOTCH_INC) $(PTSCOTCH_LIB) $(PARMETIS_INC) $(PARMETIS_LIB) -c $(C_OP2_SRC)/externlib/op_renumber.cpp -o $(F_OP2_OBJ)/op_renumber.o
	ar -r $(F_OP2_LIB)/libop2_for_mpi_cuda.a \
	$(F_OP2_OBJ)/op_lib_core.o \
	$(F_OP2_OBJ)/op_rt_support.o \
	$(F_OP2_OBJ)/op_util.o \
	$(F_OP2_OBJ)/op2_for_C_wrappers.o \
	$(F_OP2_OBJ)/op2_for_declarations.o \
	$(F_OP2_OBJ)/op2_for_hdf5_declarations.o \
	$(F_OP2_OBJ)/op2_for_rt_support.o \
	$(F_OP2_OBJ)/op2_for_rt_wrappers_cuda.o \
	$(F_OP2_OBJ)/op2_for_rt_wrappers.o \
	$(F_OP2_OBJ)/op_mpi_core.o \
	$(F_OP2_OBJ)/op_mpi_cuda_decl.o \
	$(F_OP2_OBJ)/op_cuda_rt_support.o \
	$(F_OP2_OBJ)/op_mpi_part_core.o \
	$(F_OP2_OBJ)/op_mpi_cuda_rt_support.o \
	$(F_OP2_OBJ)/op2_C_reference.o \
	$(F_OP2_OBJ)/op2_for_reference.o \
	$(F_OP2_OBJ)/op_mpi_cuda_kernels.o \
	$(F_OP2_OBJ)/op_mpi_util.o \
	$(F_OP2_OBJ)/op_renumber.o \
	$(F_OP2_OBJ)/op_mpi_hdf5.o

clean:
	-rm -if $(F_OP2_OBJ)/*.o
	-rm -if $(F_OP2_LIB)/*.a
	-rm -if $(F_OP2_SRC)/*~
	-rm -if $(F_OP2_SRC)/*~
	-rm -if $(F_OP2_SRC)/op2_for_reference.c
	-rm -if $(F_OP2_SRC)/op2_cpp_reference.F90
	-rm -if $(F_OP2_MOD)/*.mod
	-rm -if $(F_INC_MOD)/*.mod
	-rm -if $(F_OP2)/*~

