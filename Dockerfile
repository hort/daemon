FROM sptrakesh/mongocxx

ENV LIBRARY_PATH=/opt/local/lib:/usr/local/lib64
ENV LD_LIBRARY_PATH=/opt/local/lib:/usr/local/lib64
ENV PKG_CONFIG_PATH=/opt/local/lib/pkgconfig:/usr/local/lib64/pkgconfig

# Install dependencies
RUN apk update
RUN apk add \
  git cmake clang \
  re2-dev fmt-dev curl-dev libzip-dev yaml-cpp-dev \
  protobuf-dev protoc snappy

# Install loki-cpp
RUN git clone http://github.com/mircodezorzi/loki-cpp /loki-cpp
WORKDIR /loki-cpp
RUN cmake -Bbuild -H.
RUN cmake --build build --target loki-cpp -- -j4
RUN cmake --build build --target install

# Compile Daemon
COPY . /daemon
WORKDIR /daemon
RUN rm -r /daemon/build
RUN cmake -Bbuild -H.
RUN cmake --build build --target hortd -- -j4
