//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2016-20, Lawrence Livermore National Security, LLC and Umpire
// project contributors. See the COPYRIGHT file for details.
//
// SPDX-License-Identifier: (MIT)
//////////////////////////////////////////////////////////////////////////////
#ifndef REPLAY_ReplayOptions_HPP
#define REPLAY_ReplayOptions_HPP

#include <string>

struct ReplayOptions {
  bool time_replay_run{false};    // -t,--time-run
  bool time_replay_parse{false};  // --time-parse
  bool print_info{false};         // --info
  bool print_statistics{false};   // -s,--stats
  bool skip_operations{false};    // --skip-operations
  bool force_compile{false};      // -r,--recompile
  std::string input_file;         // -i,-infile input_file
  std::string pool_to_use;        // -p,--use-pool
};

#endif  // REPLAY_ReplayOptions_HPP