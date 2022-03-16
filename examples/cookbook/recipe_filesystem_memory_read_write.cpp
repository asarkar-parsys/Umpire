//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016-21, Lawrence Livermore National Security, LLC and Umpire
// project contributors. See the COPYRIGHT file for details.
//
// SPDX-License-Identifier: (MIT)
//////////////////////////////////////////////////////////////////////////////

#include "umpire/Allocator.hpp"
#include "umpire/ResourceManager.hpp"

int main(int, char** argv)
{
  // _sphinx_tag_tut_file_allocate_start
  auto& rm = umpire::ResourceManager::getInstance();
  umpire::Allocator alloc = rm.getAllocator("FILE");
  std::size_t* ptr = (std::size_t*)alloc.allocate(1024*sizeof(size_t));
  // __sphinx_tag_tut_file_allocate_end
  std::size_t* start = ptr;
  for (int i = 0; i <= 9; i++) {
  	*start = (size_t)i;
        start += sizeof(size_t);
  }
  start = ptr;
  for (int i = 0; i <= 9; i++) {
  	if ((std::size_t)i != *start) {
   	    std::cout<<"Failed comparison"<<std::endl;
            exit(-1);
        }
        start += sizeof(size_t);
  }
  // _sphinx_tag_tut_file_deallocate_start
  alloc.deallocate(ptr);
  // _sphinx_tag_tut_file_deallocate_end
  std::cout<<"Test Passed"<<std::endl;
  return 0;
}
