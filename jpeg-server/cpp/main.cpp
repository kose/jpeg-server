// Copyright (C) 2023 KOSEKI Yoshinori
///
/// @file  main.cpp
/// @brief JPEGサーバ
/// @author KOSEKI Yoshinori <koseki.y@gmail.com>
///

#include <iostream>
using std::cerr;
using std::endl;

#include <zmq.hpp>
#include <opencv2/opencv.hpp>

#include "jpeg-server.hpp"

#include <gflags/gflags.h>

DEFINE_string(port, "5555", "ZMQ server port number");
DEFINE_string(i, (std::string)HOME_DIR + "/GoogleDrive/movies/1024x600_shinyokohama2.mp4", "movie filename");
DEFINE_bool(loop, false, "Do loop?");
DEFINE_bool(flip, false, "Do flip?");


//
// main
//
int main(int argc, char *argv[])
{
  try {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    JPEGServer server(FLAGS_port, FLAGS_loop, FLAGS_flip, FLAGS_i);
  
    for (int frame_number = 0; ;frame_number++) {

      if (server.run() == false) {
        break;
      }

#if _DEBUG_
      if (frame_number % 100 == 0) {
        cerr << "frame: " << frame_number << endl;
      }
#endif
    }
    
    return EXIT_SUCCESS;
  }

  // -*- error -*- //

  catch (std::bad_alloc &e) {
    std::cerr << "BAD ALLOC Exception : " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  catch (const std::exception& e) {
    std::cerr << "Error: "  << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  catch (...) {
    std::cerr << "unknown exception" << std::endl;
    return EXIT_FAILURE;
  }
}

/// Local Variables: ///
/// truncate-lines:t ///
/// End: ///
