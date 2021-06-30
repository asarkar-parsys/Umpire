//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016-20, Lawrence Livermore National Security, LLC and Umpire
// project contributors. See the COPYRIGHT file for details.
//
// SPDX-License-Identifier: (MIT)
//////////////////////////////////////////////////////////////////////////////
#include "umpire/DeviceAllocator.hpp"
#include "umpire/Umpire.hpp"
#include "umpire/ResourceManager.hpp"

__global__ void my_kernel(umpire::DeviceAllocator alloc, double** data_ptr)
{
  if (threadIdx.x == 0) {
    //auto alloc = umpire::getDeviceAllocator(id);
    double* data = static_cast<double*>(alloc.allocate(10 * sizeof(double)));
    *data_ptr = data;

    data[7] = 1024;
  }
}

int main(int argc, char const* argv[])
{
  auto& rm = umpire::ResourceManager::getInstance();
  auto allocator = rm.getAllocator("UM");
  auto device_allocator = rm.makeDeviceAllocator(allocator, 1024);
  auto device_allocator2 = rm.makeDeviceAllocator(allocator, 2048);

  double** ptr_to_data =
      static_cast<double**>(allocator.allocate(sizeof(double*)));

  size_t id = device_allocator.getID();

  my_kernel<<<1, 16>>>(device_allocator, ptr_to_data);
  cudaDeviceSynchronize();
  //my_kernel<<<1, 16>>>(device_allocator2, ptr_to_data);
  //cudaDeviceSynchronize();
  //my_kernel<<<1, 16>>>(device_allocator3, ptr_to_data);
  //cudaDeviceSynchronize();

  std::cout << (*ptr_to_data)[7] << std::endl;
  std::cout << "DA1 with ID:" << device_allocator.getID() << std::endl;
  std::cout << "DA2 with ID:" << device_allocator2.getID() << std::endl;
  //std::cout << "DA3 with ID:" << device_allocator3.getID() << std::endl;
}
