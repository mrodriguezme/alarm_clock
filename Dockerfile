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

FROM alpine:3.24.1@sha256:28bd5fe8b56d1bd048e5babf5b10710ebe0bae67db86916198a6eec434943f8b

RUN apk update --no-cache &&                              \
    apk add --no-cache                                    \
    build-base=0.5-r4                                     \
    clang22-extra-tools=22.1.3-r2                         \
    cmake=4.2.3-r0                                        \
    doxygen=1.17.0-r0                                     \
    gdb-multiarch=16.3-r4                                 \
    graphviz=12.2.1-r3                                    \
    ninja-build=1.13.2-r1                                 \
    picolibc-arm-none-eabi=1.8.11-r0                      \
    ruby=3.4.9-r0                                      && \
    gem install ceedling -v 1.1.7 --no-document        && \
    ln -s /usr/lib/ninja-build/bin/ninja /usr/bin/ninja

ENV PATH="/usr/lib/ninja-build/bin:${PATH}"
CMD ["sh"]
