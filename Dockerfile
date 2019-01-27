FROM base/devel

RUN pacman -Sy vim yaml-cpp valgrind git bc --needed --noconfirm
RUN git clone https://github.com/Electrux/ccp4m.git
RUN cd ccp4m && ./build.sh && cp bin/ccp4m /usr/bin
