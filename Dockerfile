# SPDX-License-Identifier: MIT
#
# Copyright 2026 Michael Rodriguez
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

FROM debian:stable-slim@sha256:04634311a8d5fc442b6eb06d792293c4f3e2268652ca7634e00ce8ef5cc0a28a

RUN apt-get update                          && \
    ARCH="$(dpkg --print-architecture)"     && \
    if [ "$ARCH" = "arm64" ]; then             \
        NINJA_VER="1.12.1-1+b1";               \
    else                                       \
        NINJA_VER="1.12.1-1";                  \
    fi                                      && \
    apt-get install -y --no-install-recommends \
    build-essential=12.12                      \
    clangd=1:19.0-63                           \
    cmake=3.31.6-2                             \
    doxygen=1.9.8+ds-2.1                       \
    gcc-arm-none-eabi=15:14.2.rel1-1           \
    gdb-multiarch=16.3-1                       \
    graphviz=2.42.4-3                          \
    libicu76=76.1-4                            \
    ninja-build="$NINJA_VER"                   \
    picolibc-arm-none-eabi=1.8.10-2            \
    ruby=1:3.3+b1                           && \
    rm -rf /var/lib/apt/lists/*             && \
    gem install ceedling -v 1.1.7 --no-document

CMD ["bash"]
