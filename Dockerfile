FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    curl \
    wget \
    python3 \
    python3-pip \
    nlohmann-json3-dev \
    libboost-all-dev \
    libsqlite3-dev \
    software-properties-common \
    gnupg \
    ca-certificates \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# OpenFOAM 12 설치
RUN wget -O - https://dl.openfoam.org/gpg.key | apt-key add - && \
    add-apt-repository http://dl.openfoam.org/ubuntu && \
    apt-get update && apt-get install -y openfoam12 && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

# 컴파일 에러 수정
RUN sed -i 's/meshRequest.refinementRegions = request\["refinement_regions"\];/meshRequest.refinementRegions = request["refinement_regions"].get<std::vector<std::string>>();/' \
    /app/src/tools/snappy_mesh_tool.cpp

# 빌드
RUN /bin/bash -c "rm -rf build CMakeCache.txt CMakeFiles && \
    source /opt/openfoam12/etc/bashrc && \
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=20 && \
    cmake --build build --parallel $(nproc)"

# 🔧 핵심 수정: 조용한 실행 모드
RUN echo '#!/bin/bash' > /app/run.sh && \
    echo 'source /opt/openfoam12/etc/bashrc >/dev/null 2>&1' >> /app/run.sh && \
    echo 'exec /app/build/openfoam-mcp-server "$@"' >> /app/run.sh && \
    chmod +x /app/run.sh

CMD ["/app/run.sh"]
