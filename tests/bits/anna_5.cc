// ---------------------------------------------------------------------
//
// Copyright (C) 2002 - 2014 by the deal.II authors
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



// this program used to fail at one point in time.

#include "../tests.h"
#include <deal.II/lac/block_sparsity_pattern.h>

int main ()
{
  std::ofstream o("output");
  BlockSparsityPattern      sparsity_pattern;
}
