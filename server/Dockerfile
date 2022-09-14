FROM gcc:latest

WORKDIR /talkie-smalkie-server

COPY . .

RUN git clone https://github.com/hyper-prog/hasses.git && \
    cd hasses && make && make install   

RUN apt-get update && apt-get install -y libjson-c-dev

RUN cd src && make && chmod +x ../run_server.sh

CMD [ "/bin/bash", "./run_server.sh" ]
