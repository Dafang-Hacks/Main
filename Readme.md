## Main Repository for Low-Level Development

### Build Enviroment

You can either cross-compile from a native Ubuntu/Debian setup or  build in docker container.

#### Ubuntu / Debian (x86 cross-compile)
Install the required dependencies:

```shell
sudo apt install build-essential git gcc-mips-linux-gnu autoconf autoconf libtool libtool cmake ftp-upload u-boot-tools pkg-config mercurial
```

#### Docker (Slower!)
Build your own local docker image:
```shell
git clone --recurse-submodules https://github.com/Dafang-Hacks/Main.git
cd Main
docker build -t ${USER}/dafang-cross-compile .
docker run --rm -ti -v $(pwd):/root/ ${USER}/dafang-cross-compile:latest
 ```
 Or use the docker image provided by Daviey ( https://hub.docker.com/r/daviey/dafang-cross-compile ):
```shell
mkdir ~/dafang
cd ~/dafang
docker run --rm -ti -v $(pwd):/root/ daviey/dafang-cross-compile:latest
```
### Build
Now check out the repo with all its submodules and build the necessary libraries and then the rtspserver:
 ```shell
git clone --recurse-submodules https://github.com/Dafang-Hacks/Main.git
cd Main
./compile_modules.sh
cd v4l2rtpsserver-master
./compile.sh
```
### Develop
To update the build environment:
```shell
git submodule update --init --recursive
```
You can push the compiled binaries to your development camera via ftp-upload:
```shell
ftp-upload -h dafang.local -u root --password ismart12 -d /system/sdcard/bin/ your_file_name
```
### Sources:
- https://github.com/dim08/Ingenic-T10_20
- https://github.com/beihuijie/carrier-rtsp-server
- https://github.com/beihuijie/Ingenic-kernel
