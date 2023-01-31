#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cv2 as cv
import numpy as np
import zmq

class JPEGClient():

    def __init__(self, port="5555"):

        context = zmq.Context()
        self.socket = context.socket(zmq.REQ)
        self.socket.connect("tcp://localhost:" + port)

        print("Connecting to JPEG server…")


    def recieve(self):

        self.socket.send(b"send JPEG")  # リクエスト

        message = self.socket.recv()                       # 受信
        data = np.frombuffer(message, dtype=np.uint8) # numpy配列へ変換
        image = cv.imdecode(data, 3)                  # decode JPEG

        if 1 < len(message):
            return True, image

        return False, []


def main():

    client = JPEGClient()

    while True:
        ret, image_in = client.recieve()
        if not ret:
            break

        cv.imshow("jpeg client (python)", image_in)

        key = cv.waitKey(30)

        if key == 27:  # ESC
            break


if __name__ == '__main__':
    main()

# import pdb; pdb.set_trace()

### Local Variables: ###
### truncate-lines:t ###
### End: ###
