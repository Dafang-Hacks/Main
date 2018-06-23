FROM ubuntu:18.04

RUN \
  apt update && apt upgrade -y && \
  apt install -y \
  build-essential \
  git \
  gcc-mips-linux-gnu \
  autoconf \
  libtool \
  cmake && \
  rm -rf /var/lib/apt/lists/*

WORKDIR /root
