FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
        net-tools \
        iputils-ping \
        g++ \
        cmake \
        libtool \
        libssl-dev \
        libboost-all-dev \
        libdbus-1-dev \
        libglib2.0-dev \
        libjsoncpp-dev \
        python \
        sudo && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

CMD ["bash"]
