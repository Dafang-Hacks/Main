## Main Repository for Low-Level Development

### General Howto:
1. Clone this repository using following command:
git clone --recurse-submodules git@github.com:Dafang-Hacks/Main.git
2. Run ./compile-libraries.sh to compile all the required libraries
3. Go into v4l2rtpsserver-master and use ./compile.sh

### Build Enviroment

#### Ubuntu / Debian (x86 cross-compile)
Install required dependencies

    $ sudo apt install \
           build-essential \
           git \
           gcc-mips-linux-gnu \
           autoconf \
           libtool \
           cmake

#### Docker env
 - Use pre-prepared image (by Daviey, https://hub.docker.com/r/daviey/dafang-cross-compile )

       $ mkdir ~/defang
       $ cd ~/defang
       $ docker run --rm -ti -v $(pwd):/root/ daviey/dafang-cross-compile:latest
       $ git clone --recurse-submodules https://github.com/Dafang-Hacks/Main.git
       $ cd Main
       $ ./compile.sh
       (etc)*

 - OR Build local docker image first

        $ git clone --recurse-submodules https://github.com/Dafang-Hacks/Main.git
        $ cd Main
        $ docker build -t ${USER}/dafang-cross-compile .


### Sources:
- https://github.com/dim08/Ingenic-T10_20
- https://github.com/beihuijie/carrier-rtsp-server
- https://github.com/beihuijie/Ingenic-kernel
