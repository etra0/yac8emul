FROM devkitpro/devkitarm

RUN (cat /etc/apt/sources.list | sed 's/stretch/stable/g') >> /etc/apt/sources.list

RUN sudo apt update && sudo apt install -y g++ zip && sudo apt install -y libcurl4 cmake

RUN mkdir -p /opt/local_builds/ && mkdir -p /home/project

WORKDIR /opt/local_builds/

# Compile bannertool which is used to generate CIA files.
RUN git clone --recursive https://github.com/Steveice10/bannertool/ && \
	cd bannertool && make && cp output/linux-x86_64/bannertool $DEVKITARM/bin

RUN git clone --recursive https://github.com/3DSGuy/Project_CTR && \
	cd Project_CTR/makerom && make && cp makerom $DEVKITARM/bin

WORKDIR /home/project

ENTRYPOINT cmake -DCMAKE_TOOLCHAIN_FILE=3ds/DevkitArm3DS.cmake \
		-DBUILD_3DS=ON -Bbuild_nds && cd build_nds && make
