//
// auto-generated by op2.m on 16-Oct-2012 15:15:08
//

// user function

#include "res_calc.h"


// x86 kernel function

void op_x86_res_calc(
  int    blockIdx,
  double *ind_arg0,
  double *ind_arg1,
  double *ind_arg2,
  int   *ind_map,
  short *arg_map,
  double *arg8,
  int   *ind_arg_sizes,
  int   *ind_arg_offs,
  int    block_offset,
  int   *blkmap,
  int   *offset,
  int   *nelems,
  int   *ncolors,
  int   *colors,
  int   set_size) {

  double arg9_l[1];
  double arg10_l[1];
  double arg11_l[1];
  double arg12_l[1];
  double *arg0_vec[4];
  double *arg1_vec[4];
  double *arg2_vec[4] = {
    arg9_l,
    arg10_l,
    arg11_l,
    arg12_l
  };

  int   *ind_arg0_map, ind_arg0_size;
  int   *ind_arg1_map, ind_arg1_size;
  int   *ind_arg2_map, ind_arg2_size;
  double *ind_arg0_s;
  double *ind_arg1_s;
  double *ind_arg2_s;
  int    nelem, offset_b;

  char shared[128000];

  if (0==0) {

    // get sizes and shift pointers and direct-mapped data

    int blockId = blkmap[blockIdx + block_offset];
    nelem    = nelems[blockId];
    offset_b = offset[blockId];

    ind_arg0_size = ind_arg_sizes[0+blockId*3];
    ind_arg1_size = ind_arg_sizes[1+blockId*3];
    ind_arg2_size = ind_arg_sizes[2+blockId*3];

    ind_arg0_map = &ind_map[0*set_size] + ind_arg_offs[0+blockId*3];
    ind_arg1_map = &ind_map[4*set_size] + ind_arg_offs[1+blockId*3];
    ind_arg2_map = &ind_map[8*set_size] + ind_arg_offs[2+blockId*3];

    // set shared memory pointers

    int nbytes = 0;
    ind_arg0_s = (double *) &shared[nbytes];
    nbytes    += ROUND_UP(ind_arg0_size*sizeof(double)*2);
    ind_arg1_s = (double *) &shared[nbytes];
    nbytes    += ROUND_UP(ind_arg1_size*sizeof(double)*1);
    ind_arg2_s = (double *) &shared[nbytes];
  }

  // copy indirect datasets into shared memory or zero increment

  for (int n=0; n<ind_arg0_size; n++)
    for (int d=0; d<2; d++)
      ind_arg0_s[d+n*2] = ind_arg0[d+ind_arg0_map[n]*2];

  for (int n=0; n<ind_arg1_size; n++)
    for (int d=0; d<1; d++)
      ind_arg1_s[d+n*1] = ind_arg1[d+ind_arg1_map[n]*1];

  for (int n=0; n<ind_arg2_size; n++)
    for (int d=0; d<1; d++)
      ind_arg2_s[d+n*1] = ZERO_double;


  // process set elements

  for (int n=0; n<nelem; n++) {

    // initialise local variables

    for (int d=0; d<1; d++)
      arg9_l[d] = ZERO_double;
    for (int d=0; d<1; d++)
      arg10_l[d] = ZERO_double;
    for (int d=0; d<1; d++)
      arg11_l[d] = ZERO_double;
    for (int d=0; d<1; d++)
      arg12_l[d] = ZERO_double;

    arg0_vec[0] = ind_arg0_s+arg_map[0*set_size+n+offset_b]*2;
    arg0_vec[1] = ind_arg0_s+arg_map[1*set_size+n+offset_b]*2;
    arg0_vec[2] = ind_arg0_s+arg_map[2*set_size+n+offset_b]*2;
    arg0_vec[3] = ind_arg0_s+arg_map[3*set_size+n+offset_b]*2;

    arg1_vec[0] = ind_arg1_s+arg_map[4*set_size+n+offset_b]*1;
    arg1_vec[1] = ind_arg1_s+arg_map[5*set_size+n+offset_b]*1;
    arg1_vec[2] = ind_arg1_s+arg_map[6*set_size+n+offset_b]*1;
    arg1_vec[3] = ind_arg1_s+arg_map[7*set_size+n+offset_b]*1;

    // user-supplied kernel call


    res_calc(  arg0_vec,
               arg1_vec,
               arg8+(n+offset_b)*16,
               arg2_vec);

    // store local variables

    int arg9_map = arg_map[8*set_size+n+offset_b];
    int arg10_map = arg_map[9*set_size+n+offset_b];
    int arg11_map = arg_map[10*set_size+n+offset_b];
    int arg12_map = arg_map[11*set_size+n+offset_b];

    for (int d=0; d<1; d++)
      ind_arg2_s[d+arg9_map*1] += arg9_l[d];

    for (int d=0; d<1; d++)
      ind_arg2_s[d+arg10_map*1] += arg10_l[d];

    for (int d=0; d<1; d++)
      ind_arg2_s[d+arg11_map*1] += arg11_l[d];

    for (int d=0; d<1; d++)
      ind_arg2_s[d+arg12_map*1] += arg12_l[d];
  }

  // apply pointered write/increment

  for (int n=0; n<ind_arg2_size; n++)
    for (int d=0; d<1; d++)
      ind_arg2[d+ind_arg2_map[n]*1] += ind_arg2_s[d+n*1];

}


// host stub function

void op_par_loop_res_calc(char const *name, op_set set,
  op_arg arg0,
  op_arg arg4,
  op_arg arg8,
  op_arg arg9 ){


  int    nargs   = 13;
  op_arg args[13];

  arg0.idx = 0;
  args[0] = arg0;
  for (int v = 1; v < 4; v++) {
    args[0 + v] = op_arg_dat(arg0.dat, v, arg0.map, 2, "double", OP_READ);
  }
  arg4.idx = 0;
  args[4] = arg4;
  for (int v = 1; v < 4; v++) {
    args[4 + v] = op_arg_dat(arg4.dat, v, arg4.map, 1, "double", OP_READ);
  }
  args[8] = arg8;
  arg9.idx = 0;
  args[9] = arg9;
  for (int v = 1; v < 4; v++) {
    args[9 + v] = op_arg_dat(arg9.dat, v, arg9.map, 1, "double", OP_INC);
  }

  int    ninds   = 3;
  int    inds[13] = {0,0,0,0,1,1,1,1,-1,2,2,2,2};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: res_calc\n");
  }

  // get plan

  #ifdef OP_PART_SIZE_0
    int part_size = OP_PART_SIZE_0;
  #else
    int part_size = OP_part_size;
  #endif

  int set_size = op_mpi_halo_exchanges(set, nargs, args);

  // initialise timers

  double cpu_t1, cpu_t2, wall_t1=0, wall_t2=0;
  op_timing_realloc(0);
  OP_kernels[0].name      = name;
  OP_kernels[0].count    += 1;

  if (set->size >0) {

    op_plan *Plan = op_plan_get(name,set,part_size,nargs,args,ninds,inds);

    op_timers_core(&cpu_t1, &wall_t1);

    // execute plan

    int block_offset = 0;

    for (int col=0; col < Plan->ncolors; col++) {
      if (col==Plan->ncolors_core) op_mpi_wait_all(nargs, args);

      int nblocks = Plan->ncolblk[col];

#pragma omp parallel for
      for (int blockIdx=0; blockIdx<nblocks; blockIdx++)
      op_x86_res_calc( blockIdx,
         (double *)arg0.data,
         (double *)arg4.data,
         (double *)arg9.data,
         Plan->ind_map,
         Plan->loc_map,
         (double *)arg8.data,
         Plan->ind_sizes,
         Plan->ind_offs,
         block_offset,
         Plan->blkmap,
         Plan->offset,
         Plan->nelems,
         Plan->nthrcol,
         Plan->thrcol,
         set_size);

      block_offset += nblocks;
    }

  op_timing_realloc(0);
  OP_kernels[0].transfer  += Plan->transfer;
  OP_kernels[0].transfer2 += Plan->transfer2;

  }


  // combine reduction data

  op_mpi_set_dirtybit(nargs, args);

  // update kernel record

  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[0].time     += wall_t2 - wall_t1;
}

