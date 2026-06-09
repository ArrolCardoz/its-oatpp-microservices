FROM janvik/oatpp:latest

WORKDIR /
ADD . /shapeREST
WORKDIR ./shapeREST

RUN make testServer
RUN ./testServer

EXPOSE 8000 8000

RUN make shapeServer
ENTRYPOINT ["./shapeServer"]

# Used to allow for connection to container so can use commandline
#ENTRYPOINT ["sleep", "infinity"]