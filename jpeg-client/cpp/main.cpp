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

#include "jpeg-client.hpp"

#include <gflags/gflags.h>

DEFINE_string(port, "5555", "ZMQ client port number");


//
// main
//
int main(int argc, char *argv[])
{
  try {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    JPEGClient client(FLAGS_port);
    cv::Mat image_in;
    
    while (cv::waitKey(33) != 27) { // ESC exit

      client.run(image_in);

      if (image_in.empty()) {
        break;
      }

      cv::imshow("jpeg client (cpp)", image_in);
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
