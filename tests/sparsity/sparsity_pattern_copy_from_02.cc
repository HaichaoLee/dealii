// ---------------------------------------------------------------------
//
// Copyright (C) 2016 by the deal.II authors
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



// SparsityPattern::copy_from used to be of quadratic complexity for
// DynamicSparsityPattern arguments with many empty rows. Create a big
// sparsity pattern of 10m elements and only put an entry into the last
// row. The old code would time out.

#include "../tests.h"
#include <deal.II/lac/sparsity_pattern.h>
#include <deal.II/lac/dynamic_sparsity_pattern.h>
#include <list>
#include <set>


int
main ()
{
  std::ofstream logfile("output");
  logfile.setf(std::ios::fixed);
  deallog << std::setprecision(3);
  deallog.attach(logfile);

  DynamicSparsityPattern csp (10000000, 100);
  csp.add(9999999, 1);

  SparsityPattern sp;
  sp.copy_from (csp);

  deallog << "OK" << std::endl;
}
