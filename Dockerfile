FROM debian:bullseye-slim AS build

RUN uname -m > /arch

# common packages used throughout
RUN apt update
RUN apt install -y build-essential cmake csh


WORKDIR /workspace

COPY ./cspice-fork/ ./cspice-fork
WORKDIR /workspace/cspice-fork

RUN mkdir mkdir ./exe && mkdir ./lib

RUN chmod +x ./makeall.csh && csh ./makeall.csh



