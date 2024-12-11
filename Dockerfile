FROM debian:bullseye-slim AS build

RUN uname -m > /arch

# common packages used throughout
RUN apt-get update
RUN apt-get install -y build-essential csh

WORKDIR /workspace

COPY ./cspice-fork ./cspice-fork
WORKDIR /workspace/cspice-fork

RUN mkdir mkdir ./exe && mkdir ./lib

RUN chmod +x ./makeall.csh && csh ./makeall.csh



