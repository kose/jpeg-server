// Copyright (C) 2023 KOSEKI Yoshinori
///
/// @file  jpege-server.hpp
/// @brief JPEGサーバ
/// @author KOSEKI Yoshinori <koseki.y@gmail.com>
///

#pragma once

#include <iostream>

//
// MJPEGサーバクラス
//
class JPEGServer {
public:

  // コンストラクタ
  JPEGServer (const std::string port, const bool loop, const bool flip, const std::string moviefile);

  // デストラクタ
  virtual ~JPEGServer();

  // 1フレーム処理
  bool run();

private:

  zmq::context_t *context;      ///< ZeroMQ context
  zmq::socket_t *socket;        ///< ZeroMQソケット

  cv::VideoCapture capture;     ///< ビデオキャプチャ
  cv::Mat image_in;             ///< 入力画像

  const bool loop;              ///< ループするか?
  const bool flip;              ///< フリップするか?

  int frame_number;             ///< 出力フレーム番号
};

/// Local Variables: ///
/// truncate-lines:t ///
/// End: ///
