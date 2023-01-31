#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import argparse
import cv2 as cv
import numpy as np
import zmq

moviefile=os.path.expanduser("~/GoogleDrive/movies/1024x600_shinyokohama2.mp4")

parser = argparse.ArgumentParser(description='jpeg server')
parser.add_argument('--port', type=str, default="5555", help='port number')
parser.add_argument('-i', type=str, default=moviefile, help='movie filename')
args = parser.parse_args()

class JPEGServer():

    def __init__(self, capture, port="5555"):

        self.capture = capture
        
        context = zmq.Context()
        self.socket = context.socket(zmq.REP)
        self.socket.bind("tcp://*:" + port)

        print("start JPEG server")


    def run(self):

        ret, image_in = self.capture.read()

        if ret:
            _, encoded = cv.imencode(".jpg", image_in, (cv.IMWRITE_JPEG_QUALITY, 90))

        message = self.socket.recv() # FIXME: requestの確認必要

        if not ret:
            self.socket.send(b" ")
            return False

        self.socket.send(encoded)

        return True


def main():

    if args.i == "0":
        device = 0
    else:
        device = args.i

    capture = cv.VideoCapture(device)
    
    server = JPEGServer(capture)

    ret = True
    
    while ret:
        ret = server.run()


if __name__ == '__main__':
    main()
        
# import pdb; pdb.set_trace()

### Local Variables: ###
### truncate-lines:t ###
### End: ###
