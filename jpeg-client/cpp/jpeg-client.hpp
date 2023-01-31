// Copyright (C) 2023 KOSEKI Yoshinori
///
/// @file  jpege-client.hpp
/// @brief JPEGサーバ
/// @author KOSEKI Yoshinori <koseki.y@gmail.com>
///

#pragma once

#include <iostream>

//
// MJPEGサーバクラス
//
class JPEGClient {
public:

  // コンストラクタ
  JPEGClient (const std::string port);

  // デストラクタ
  virtual ~JPEGClient();

  // 1フレーム受信
  void run(cv::Mat& image_proc);

private:

  zmq::context_t *context;      ///< ZeroMQ context
  zmq::socket_t *socket;        ///< ZeroMQソケット
};

/// Local Variables: ///
/// truncate-lines:t ///
/// End: ///
