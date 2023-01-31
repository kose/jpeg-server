// Copyright (C) 2023 KOSEKI Yoshinori
///
/// @file  jpege-server.cpp
/// @brief JPEGサーバ
/// @author KOSEKI Yoshinori <koseki.y@gmail.com>
///

#include <iostream>
#include <zmq.hpp>
#include <opencv2/opencv.hpp>

#include "jpeg-server.hpp"

// コンストラクタ
JPEGServer::JPEGServer (const std::string port, const bool loop, const bool flip, const std::string moviefile) :
    loop(loop), flip(flip)
{
  context = new zmq::context_t(1);
  socket = new zmq::socket_t(*context, ZMQ_REP);
  socket->bind("tcp://*:" + port);

  std::cerr << "start JPEG server" << std::endl;
  
  if (moviefile == "0") { // camera
    capture.open(0);
  } else {
    capture.open(moviefile);    // movie file
  }

  if (!capture.isOpened()) {
    throw std::runtime_error("Can not open VideoCapture: ");
  }

  capture >> image_in;
    
  frame_number = 0;
}

// デストラクタ
JPEGServer::~JPEGServer()
{
}

// 1フレーム処理
bool JPEGServer::run()
{
  // receive pull request
  {
    zmq::message_t message_recv;

    // receive: data
    socket->recv(&message_recv);

    int size = message_recv.size();
    char buff[size];
    memcpy(&buff[0], (unsigned char*)message_recv.data(), size);

    // FIXME: requestの確認必要
  }

  //
  // input image
  //
  if (image_in.empty()) {
    if (loop) {
      std::cerr << "rewind " << frame_number << std::endl;

      capture.set(cv::CAP_PROP_POS_FRAMES, 0); // 終端なら巻き戻し
      capture >> image_in;

    } else {
      std::cerr << "------ end of movie: " << frame_number <<  " ----- " << std::endl;

      // send JPEG buffer (empty image)
      zmq::message_t message_send(1);
      socket->send(message_send);
      return false;
    }

    if (flip) {
      cv::flip(image_in, image_in, 1);
    }
  }

  //
  // send JPEG stream
  //

  // make jpeg
  std::vector<unsigned char> buff;
  std::vector<int> param = std::vector<int>(2);
  param[0] = cv::IMWRITE_JPEG_QUALITY;
  param[1] = 90;                // default(95) 0-100
  imencode(".jpg", image_in, buff, param);

  // make JPEG buffer
  zmq::message_t message_send(buff.size());
  memcpy(message_send.data(), &buff[0], buff.size());

  socket->send(message_send);

#if _DEBUG_
  cv::imshow("server foreground", image_in);

  if (cv::waitKey(1) == 27) {
    throw std::runtime_error("ESC: exit server");
  }
#endif

  capture >> image_in;
  frame_number++;
    
  return true;
}

/// Local Variables: ///
/// truncate-lines:t ///
/// End: ///
