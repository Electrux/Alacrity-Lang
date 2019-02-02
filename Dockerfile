FROM base/devel:2019.01.01

RUN pacman -Sy vim yaml-cpp valgrind git bc --needed --noconfirm
RUN git clone https://github.com/Electrux/ccp4m.git
WORKDIR /ccp4m
RUN ./build.sh && cp bin/ccp4m /usr/bin
