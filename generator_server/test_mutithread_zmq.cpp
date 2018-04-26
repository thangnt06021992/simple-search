
#include <thread>
//#include <pthread.h>
#include <unistd.h>
#include <cassert>
#include <string>
#include <iostream>
#include <zmq.hpp>

void *worker_routine (void *arg)
{
    zmq::context_t *context = (zmq::context_t *) arg;

    zmq::socket_t socket (*context, ZMQ_REP);
    socket.connect ("inproc://dealer");

    while (true) {
        //  Wait for next request from client
        zmq::message_t request;
        socket.recv (&request);
        std::cout << "Received request: [" << (char*) request.data() << "]" << std::endl;

        //  Do some 'work'
       // sleep (1);

        //  Send reply back to client
        zmq::message_t reply (6);
        memcpy ((void *) reply.data (), "World", 6);
        socket.send (reply);
    }
    return (NULL);
}

int main (int argc, char* argv[])
{
    //  Prepare our context and sockets
    zmq::context_t context (1);
    zmq::socket_t clients (context, ZMQ_ROUTER);
    clients.bind ("tcp://*:5555");
    zmq::socket_t workers (context, ZMQ_DEALER);
    workers.bind ("inproc://dealer");

    //  Launch pool of worker threads
    for (int thread_nbr = 0; thread_nbr != 5; thread_nbr++) {
        //pthread_t worker;
        //pthread_create (&worker, NULL, worker_routine, (void *) &context);
	//std::thread thread(worker_routine, (void*)&context);
	std::thread* thread = new  std::thread(worker_routine, (void*)&context);
    }
    //  Connect work threads to client threads via a queue
    zmq::proxy (clients, workers, NULL);
    return 0;
}