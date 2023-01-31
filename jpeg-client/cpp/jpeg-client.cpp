// Copyright (C) 2023 KOSEKI Yoshinori
///
/// @file  jpege-client.cpp
/// @brief JPEGサーバ
/// @author KOSEKI Yoshinori <koseki.y@gmail.com>
///

#include <iostream>
#include <zmq.hpp>
#include <opencv2/opencv.hpp>

#include "jpeg-client.hpp"

// コンストラクタ
JPEGClient::JPEGClient (const std::string port)
{
  context = new zmq::context_t(1);
  socket = new zmq::socket_t(*context, ZMQ_REQ);
  socket->connect ("tcp://localhost:" + port);

  std::cerr << "Connecting to JPEG server…" << std::endl;
}

// デストラクタ
JPEGClient::~JPEGClient()
{
}

// 1フレーム受信
void JPEGClient::run(cv::Mat& image_proc)
{
  { // request
    std::string request = "send JPEG";

    int size = request.length();
    zmq::message_t message(size);
    memcpy(message.data(), request.c_str(), size);
    socket->send(message);
  }

  // get jpeg
  {
    zmq::message_t message;

    // recieve: data
    socket->recv(&message);

    int size = message.size();
    std::vector<unsigned char> buff(size);
    memcpy(&buff[0], (unsigned char*)message.data(), size);
    image_proc = cv::imdecode(cv::Mat(buff), 1); // CV_LOAD_IMAGE_COLOR
  }
}

/// Local Variables: ///
/// truncate-lines:t ///
/// End: ///
