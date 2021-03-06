// ---------------------------------------------------------------------
//
// Copyright (C) 2017 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------

#ifndef dealii_scalapack_templates_h
#define dealii_scalapack_templates_h


#include <deal.II/base/config.h>

#ifdef DEAL_II_WITH_SCALAPACK

#include <deal.II/base/mpi.h>
#include <deal.II/base/mpi.templates.h>

// useful examples:
// https://stackoverflow.com/questions/14147705/cholesky-decomposition-scalapack-error/14203864
// http://icl.cs.utk.edu/lapack-forum/viewtopic.php?t=139   // second post by Julien Langou
// https://andyspiros.wordpress.com/2011/07/08/an-example-of-blacs-with-c/
// http://qboxcode.org/trac/browser/qb/tags/rel1_63_4/src/Matrix.C
// https://gitlab.phys.ethz.ch/lwossnig/lecture/blob/a534f562dfb2ad5c564abe5c2356d5d956fb7218/examples/mpi/scalapack.cpp
// https://github.com/elemental/Elemental/blob/master/src/core/imports/scalapack.cpp
// https://scicomp.stackexchange.com/questions/7766/performance-optimization-or-tuning-possible-for-scalapack-gemm
//
// info:
// http://www.netlib.org/scalapack/slug/index.html       // User guide
// http://www.netlib.org/scalapack/slug/node135.html // How to Measure Errors

extern "C"
{
  /* Basic Linear Algebra Communication Subprograms (BLACS) declarations */
  // https://www.ibm.com/support/knowledgecenter/SSNR5K_4.2.0/com.ibm.cluster.pessl.v4r2.pssl100.doc/am6gr_dinitb.htm#dinitb

  /**
   * Determine how many processes are available and the current process rank.
   *
   * https://www.ibm.com/support/knowledgecenter/en/SSNR5K_4.2.0/com.ibm.cluster.pessl.v4r2.pssl100.doc/am6gr_dbpnf.htm
   */
  void Cblacs_pinfo(int *rank, int *nprocs);

  /**
   * Return internal BLACS value in @p val based on the input @p what and @p icontxt.
   * The most common use is in retrieving a default system context (@p what = 0, @p icontxt is ignored)
   * to be used in BLACS_GRIDINIT or BLACS_GRIDMAP.
   *
   * https://www.ibm.com/support/knowledgecenter/en/SSNR5K_4.2.0/com.ibm.cluster.pessl.v4r2.pssl100.doc/am6gr_dbget.htm
   */
  void Cblacs_get(int icontxt, int what, int *val);

  /**
   * Map the processes sequentially in row-major or column-major order
   * into the process grid. Input arguments must be the same on every process.
   *
   * On return, @p context is the integer handle to the BLACS context,
   * whereas on entry it is a system context to be used in creating the
   * BLACS context.
   *
   * https://www.ibm.com/support/knowledgecenter/en/SSNR5K_4.2.0/com.ibm.cluster.pessl.v4r2.pssl100.doc/am6gr_dbint.htm
   */
  void Cblacs_gridinit(int *context, const char *order, int grid_height, int grid_width);

  /**
   * Return the process row and column index.
   *
   * https://www.ibm.com/support/knowledgecenter/en/SSNR5K_4.2.0/com.ibm.cluster.pessl.v4r2.pssl100.doc/am6gr_dbinfo.htm
   */
  void Cblacs_gridinfo(int context, int *grid_height, int *grid_width, int *grid_row, int *grid_col);

  /**
   * Given the system process number, return the row and column coordinates in the BLACS' process grid.
   */
  void Cblacs_pcoord(int ictxt, int pnum, int *prow, int *pcol);

  /**
   * Release a BLACS context.
   */
  void Cblacs_gridexit(int context);

  /**
   * This routines holds up execution of all processes within the indicated
   * scope until they have all called the routine.
   */
  void Cblacs_barrier(int, const char *);

  /**
   * Free all BLACS contexts and releases all allocated memory.
   */
  void Cblacs_exit(int error_code);

  /**
   * Receives a message from a process @prsrc, @p csrc into a general rectangular matrix.
   *
   * https://software.intel.com/en-us/mkl-developer-reference-c-gerv2d
   */
  void Cdgerv2d(int context, int M, int N, double *A, int lda, int rsrc, int csrc);

  /**
   * Sends the general rectangular matrix A to the destination
   * process @p rdest @p cdest in the process grid.
   *
   * https://software.intel.com/en-us/mkl-developer-reference-c-2018-beta-gesd2d
   */
  void Cdgesd2d(int context , int M, int N, double *A, int lda, int rdest, int cdest);

  /**
   * Get BLACS context from MPI @p comm.
   */
  int Csys2blacs_handle(MPI_Comm comm);

  /**
   * Compute how many rows and columns each process owns (NUMber of Rows Or Columns).
   *
   * https://www.ibm.com/support/knowledgecenter/SSNR5K_4.2.0/com.ibm.cluster.pessl.v4r2.pssl100.doc/am6gr_dnumy.htm
   */
  int numroc_ (const int *n, const int *nb, const int *iproc, const int *isproc, const int *nprocs);

  /**
   * Compute the Cholesky factorization of an N-by-N real
   * symmetric positive definite distributed matrix sub( A ) denoting
   * A(IA:IA+N-1, JA:JA+N-1).
   *
   * http://www.netlib.org/scalapack/explore-html/d5/d9e/pdpotrf_8f_source.html
   * https://www.ibm.com/support/knowledgecenter/SSNR5K_4.2.0/com.ibm.cluster.pessl.v4r2.pssl100.doc/am6gr_lpotrf.htm
   */
  void pdpotrf_(const char *UPLO,
                const int *N,
                double *A, const int *IA, const int *JA, const int *DESCA,
                int *INFO);

  /**
   * Compute the inverse of a real symmetric positive definite
   * distributed matrix sub( A ) = A(IA:IA+N-1,JA:JA+N-1) using the
   * Cholesky factorization sub( A ) = U**T*U or L*L**T computed by
   * PDPOTRF.
   *
   * http://www.netlib.org/scalapack/explore-html/d2/d44/pdpotri_8f_source.html
   * https://www.ibm.com/support/knowledgecenter/SSNR5K_4.2.0/com.ibm.cluster.pessl.v4r2.pssl100.doc/am6gr_lpotri.htm
   * https://software.intel.com/en-us/mkl-developer-reference-c-p-potri
   */
  void pdpotri_(const char *UPLO,
                const int *N,
                double *A, const int *IA, const int *JA, const int *DESCA,
                int *INFO);

  /**
   * Estimate the reciprocal of the condition number (in the
   * l1-norm) of a real symmetric positive definite distributed matrix
   * using the Cholesky factorization.
   *
   * https://www.ibm.com/support/knowledgecenter/SSNR5K_4.2.0/com.ibm.cluster.pessl.v4r2.pssl100.doc/am6gr_lpocon.htm#lpocon
   * http://www.netlib.org/scalapack/explore-html/d4/df7/pdpocon_8f.html
   * https://software.intel.com/en-us/mkl-developer-reference-fortran-pocon
   */
  void pdpocon_(const char *uplo,
                const int *N,
                const double *A, const int *IA, const int *JA, const int *DESCA,
                const double *ANORM, double *RCOND,
                double *WORK, const int *LWORK,
                int *IWORK, const int *LIWORK,
                int *INFO);

  /**
   * Norm of a real symmetric matrix
   *
   * http://www.netlib.org/scalapack/explore-html/dd/d12/pdlansy_8f_source.html
   * https://www.ibm.com/support/knowledgecenter/SSNR5K_4.2.0/com.ibm.cluster.pessl.v4r2.pssl100.doc/am6gr_pdlansy.htm#pdlansy
   */
  double pdlansy_(const char *norm,
                  const char *uplo,
                  const int *N,
                  const double *A, const int *IA, const int *JA, const int *DESCA,
                  double *work);

  /**
   * Compute the Least Common Multiple (LCM) of two positive integers @p M and @p N.
   * In fact the routine Compute the greatest common divisor (GCD) and
   * use the fact that M*N = GCD*LCM.
   *
   * http://www.netlib.org/scalapack/explore-html/d0/d9b/ilcm_8f_source.html
   */
  int ilcm_(const int *M, const int *N);

  /**
   * Return the ceiling of the division of two integers.
   *
   * http://www.netlib.org/scalapack/explore-html/df/d07/iceil_8f_source.html
   */
  int iceil_(const int *i1, const int *i2);

  /**
   * Initialize the descriptor vector with the 8 input arguments
   */
  void descinit_ (int *desc,
                  const int *m, const int *n, const int *mb, const int *nb,
                  const int *irsrc, const int *icsrc,
                  const int *ictxt, const int *lld, int *info);

  /**
   * Compute the global index of a distributed matrix entry
   * pointed to by the local index @p indxloc of the process indicated by
   * @p iproc.
   *
   * @param indxloc The local index of the distributed matrix entry.
   * @param nb Block size, size of the blocks the distributed matrix is split into.
   * @param iproc The coordinate of the process whose local array row or column is to be determined
   * @param isrcproc  The coordinate of the process that possesses the first row/column of the distributed matrix
   * @param nprocs The total number processes over which the distributed matrix is distributed
   */
  int indxl2g_ (const int *indxloc, const int *nb, const int *iproc, const int *isrcproc, const int *nprocs);

  /**
   * Compute the solution to a real system of linear equations
   */
  void pdgesv_(const int *n, const int *nrhs,
               double *A, const int *ia, const int *ja, const int *desca,
               int *ipiv,
               double *B, const int *ib, const int *jb, const int *descb,
               int *info);

  /**
   * Perform one of the matrix-matrix operations:
   * sub( C ) := alpha*op( sub( A ) )*op( sub( B ) ) + beta*sub( C ),
   * where
   * sub( C ) denotes C(IC:IC+M-1,JC:JC+N-1),  and, op( X )  is one  of
   * op( X ) = X   or   op( X ) = X'.
   */
  void pdgemm_(const char *transa, const char *transb,
               const int *m, const int *n, const int *k,
               const double *alpha,
               double *A, const int *IA, const int *JA, const int *DESCA,
               double *B, const int *IB, const int *JB, const int *DESCB,
               const double *beta,
               double *C, const int *IC, const int *JC, const int *DESCC);

  /**
   * Return the value of the one norm, or the Frobenius norm, or the infinity norm,
   * or the element of largest absolute value of a distributed matrix
   */
  double pdlange_(char const *norm,
                  int const &m, int const &n,
                  double *A, int const &ia, int const &ja, int *desca,
                  double *work);

  /**
   * Compute the process coordinate which possesses the entry of a
   * distributed matrix specified by a global index
   */
  int indxg2p_(const int *glob, const int *nb, const int *iproc, const int *isproc, const int *nprocs);

  /**
   * Compute all eigenvalues and, optionally, eigenvectors of a real symmetric matrix A
   * by calling the recommended sequence of ScaLAPACK routines. In its present form, the routine assumes a homogeneous system
   * and makes no checks for consistency of the eigenvalues or eigenvectors across the different processes.
   * Because of this, it is possible that a heterogeneous system may return incorrect results without any error messages.
   *
   * http://www.netlib.org/scalapack/explore-html/d0/d1a/pdsyev_8f.html
   * https://www.ibm.com/support/knowledgecenter/SSNR5K_4.2.0/com.ibm.cluster.pessl.v4r2.pssl100.doc/am6gr_lsyev.htm#lsyev
   */
  void pdsyev_(const char *jobz, const char *uplo,
               const int *m, double *A, const int *ia, const int *ja, int *desca,
               double *w,
               double *z, const int *iz, const int *jz, int *descz,
               double *work, const int *lwork, int *info);

  /**
   * Copy all or a part of a distributed matrix A to another
   * distributed matrix B. No communication is performed, pdlacpy
   * performs a local copy sub(A) := sub(B), where sub(A) denotes
   * A(ia:ia+m-1,ja:ja+n-1) and sub(B) denotes B(ib:ib+m-1,jb:jb+n-1)
   */
  void pdlacpy_(const char *uplo,
                const int *m, const int *n, double *A, const int *ia, const int *ja, int *desca,
                double *B, const int *ib, const int *jb, int *descb);
}

#endif // DEAL_II_WITH_SCALAPACK

#endif // dealii_scalapack_templates_h
