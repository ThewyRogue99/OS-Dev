FROM ubuntu

RUN apt-get update
RUN apt-get install -y g++
RUN apt-get install -y nasm
RUN apt-get install -y make
RUN apt-get install -y iat

RUN mkdir /home/os-tutorial
WORKDIR /home/os-tutorial

# To build:
#   docker build -t os-buildenv buildenv

# To run:
#   docker run -it -v "%cd%":/home/os-tutorial os-buildenv