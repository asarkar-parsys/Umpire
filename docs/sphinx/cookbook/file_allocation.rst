.. _file_allocation:

==================================
Using File System Allocator (FILE)
==================================

Umpire supports the use of file based memory allocation. When ``UMPIRE_ENABLE_FILE_RESOURCE`` 
is enabled, the environment variables ``UMPIRE_MEMORY_FILE_DIR`` can be used to determine 
where memory can be allocated from:

  ======================   ======================   =======================================================
  Variable                 Default                  Description
  ======================   ======================   =======================================================
  UMPIRE_MEMORY_FILE_DIR   ./                       Directory to create and allocate file based allocations
  ======================   ======================   =======================================================

Requesting the allocation takes two steps: 1) getting a "FILE" allocator, 
2) requesting the amount of memory to allocate.

.. literalinclude:: ../../../examples/cookbook/recipe_filesystem_memory_allocation.cpp
   :start-after: _sphinx_tag_tut_file_allocate_start
   :end-before: _sphinx_tag_tut_file_allocate_end
   :language: C++

To deallocate:

.. literalinclude:: ../../../examples/cookbook/recipe_filesystem_memory_allocation.cpp
   :start-after: _sphinx_tag_tut_file_deallocate_start
   :end-before: _sphinx_tag_tut_file_deallocate_end
   :language: C++

The complete example is included below:

.. literalinclude:: ../../../examples/cookbook/recipe_filesystem_memory_allocation.cpp

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Using Burst Buffers On Lassen
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

On Lassen, 1) Download the latest version of Umpire 2) request a private node to build:

.. code-block:: bash

  $ git clone --recursive https://github.com/LLNL/Umpire.git
  $ lalloc 1 -stage storage=64

Note that ``-stage storage=64`` is needed in order to work with the Burst Buffers. 
3) Additionally, the environment variable needs to set to ``$BBPATH`` :

.. code-block:: bash

  $ export UMPIRE_MEMORY_FILE_DIR=$BBPATH/

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Running File Resource Benchmarks
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Continue building Umpire on 1 node, and set the ``-DUMPIRE_ENABLE_FILE_RESOURCE=On`` :

.. code-block:: bash

  $ mkdir build && cd build
  $ lrun -n 1 cmake -DUMPIRE_ENABLE_FILE_RESOURCE=On -DENABLE_OPENMP=On ../ && make

To run the built-in benchmarks in Umpire from the build run:

.. code-block:: bash

  $ lrun -n 1 --threads=** ./bin/file_resource_benchmarks ##

In the snippet above, "**" is a placeholder for the amount of threads wanted to run the benchmark on. 
"##" stands for the number of array elements wanted to be passed through the benchmark. 
This number can range from 1-100,000,000,000.

Results should appear like:

.. code-block:: bash
  
  Array Size:   1        Memory Size: 8e-06 MB
  Total Arrays: 3               Total Memory Size: 2.4e-05 MB

  HOST
    Initialization:      0.0247461 MB/sec
    Initialization Time: 0.000969849 sec
    ---------------------------------------
    Copy:                0.890918 MB/sec
    Copy Time:           1.7959e-05 sec
    ---------------------------------------
    Scale:               0.928074 MB/sec
    Scale Time:          1.724e-05 sec
    ---------------------------------------
    Add:                 1.321 MB/sec
    Add Time:            1.8168e-05 sec
    ---------------------------------------
    Triad:               1.24102 MB/sec
    Triad Time:          1.9339e-05 sec
    ---------------------------------------
    Total Time:          0.00104323 sec

  FILE
    Initialization:      0.210659 MB/sec
    Initialization Time: 0.000113928 sec
    ---------------------------------------
    Copy:                0.84091 MB/sec
    Copy Time:           1.9027e-05 sec
    ---------------------------------------
    Scale:               0.938086 MB/sec
    Scale Time:          1.7056e-05 sec
    ---------------------------------------
    Add:                 1.28542 MB/sec
    Add Time:            1.8671e-05 sec
    ---------------------------------------
    Triad:               1.54689 MB/sec
    Triad Time:          1.5515e-05 sec
    ---------------------------------------
    Total Time:          0.000184726 sec

This benchmark run similar to the STREAM Benchmark test and can also run a benchmark 
for the additional allocators like ``UM`` for CUDA and ``DEVICE`` for HIP.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Using File Resource with UMap
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Umpire supports the use of file based memory allocation backed by UMap memory management.
UMap is a Userfaultfd based userspace file-backed memory mapping utility that seamlessly
replaces system mmap. Using runtime environment variables UMap allows the flexibility for
the users to control mapped region's buffer size, granularity at which the file is being
accessed, etc. More details can be found at this `link`_.

.. _link: https://llnl-umap.readthedocs.io/en/develop/environment_variables.html

UMap can be used along with the file resource by building UMPIRE with UMap.
This requires a UMap installation, ``UMAP_ROOT`` environment variable pointing to the
base directory of UMap and enabling ``UMPIRE_ENABLE_UMAP`` build flag alongside file
resource build parameters ``UMPIRE_ENABLE_FILE_RESOURCE`` and  environment variables
``UMPIRE_MEMORY_FILE_DIR``.

  ======================   ======================   =======================================================
  UMap Build Parameters          Default                  Description
  ======================   ======================   =======================================================
  UMPIRE_ENABLE_UMAP       Off                      Flag to enable building UMPIRE with UMap calls
  UMAP_ROOT (env)          None Defined             Umap base directory
  ======================   ======================   =======================================================

Userfaultfd is an actively developed Linux kernel feature. To use UMPIRE with UMap'ed buffers, Userfaultfd's
Write-Protect capability is needed. Please refer to the system and runtime requirements listed `here`_.

.. _here: https://github.com/LLNL/umap

Once the system dependencies are met, follow the steps documented at the following link to build and install UMap.

https://llnl-umap.readthedocs.io/en/develop/getting_started.html

The following command lines should help you build UMPIRE with UMap. Please replace <Umap base directory>
with the absolute path to UMap base directory.

.. code-block:: bash
  $ export UMAP_ROOT=<Umap base directory>
  $ git clone https://github.com/asarkar-parsys/Umpire
  $ cd Umpire
  $ git checkout feature/integrate_umap_b2
  $ git submodule init
  $ git submodule update
  $ mkdir build && cd build
  $ cmake3 -DUMPIRE_ENABLE_FILE_RESOURCE=On -DENABLE_OPENMP=On -DUMPIRE_ENABLE_UMAP=On -DCMAKE_INSTALL_PREFIX=./install ../ && make && make install

Note that these lines will install the Umpire library and headers under Umpire/build/install/lib and Umpire/build/install/include
directories, respectively. Alongwith the knowledge of UMap library installation a standalone example application like the one
provided below can be built.

.. literalinclude:: ../../../examples/cookbook//recipe_filesystem_memory_read_write.cpp

A command line like the following is all you need to build this app

.. code-block:: bash
  $ g++ -I<UMPIRE_ROOT>/Umpire/build/install/include test.c <UMPIRE_ROOT>/Umpire-test/Umpire/build/install/lib/libumpire.a <UMAP_ROOT>/umap/install/lib/libumap.a -lpthread -o testrun

Additionally, Umpire's built-in file resource unit tests can also be run using

.. code-block:: bash
  $<UMPIRE_ROOT>/Umpire/build/install/bin/file_resource_tests ##

Here "##" stands for the number of array elements wanted to be passed through the tands for the
number of array elements wanted to be passed through the tests. This number can range from
1-100,000,000,000. This number can range from 1-100,000,000,000.
