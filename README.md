# FreeYourSoul_MQ
##FreeyourSoul LockFreeMQ 

FySMQ is a header only toolset (bus+MQ) used by the FreeYourSoul project.

**Lock-Free queue MPOC (MultiProducer, OneConsumer)**

The fys project is managing actions by using lock-free queue. Actions are stored in the queues and then consumed by a thread launch by a [buslistener](https://github.com/FreeYourSoul/FreeYourSoul_MQ/blob/master/include/bus/BusListener.hh).
Those queue use only atomic indexes (to produce elements), the content of the queue isn't atomic

**Bus**

LockFree queues can be managed by a bus that will redirect messages on the good queues thanks to index of the queue.  
The QueueContainer contains an element called ``_opCodeMessage`` that is redirecting the message to the correct queue.

**Listener**

A template class called  
```cpp
fys::mq::BusListener< ListenerClass, FySBus< TypeMessageClass, FixedSizeOfEachQueueInBus>>
```   
is used to listen to a bus queue, the ListenerClass template has to contains a public anonymous enum (enum hack) with an element called IndexInBus that is going to have the value of the index of the queue to listen to on the bus.

_usage_ :  Create a queue of a generic type 

```cpp
class MyListenerClass {
    public:
        enum { IndexInBus = 0 }; // Index of the queue in the bus to listen to is 0
}

using fys::mq;
using Listener = BusListener< MyListenerClass, FysBus< MessageTypeClass, 100>>;

int main(int argc, const char * const *argv) {
    // Instantiate a bus
    auto fysBus = std::make_shared< FysBus< MessageTypeClass, 100>> (1);
    // Instantiate a listener
    Listener listener;
    
    // listener launch a thread that is going to read continuously on the MyListenerClass::IndexInBus queue of the bus
    listener.launchListenThread(bus); 
}
 
```
