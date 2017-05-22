#include <iostream>
#include <thread>
#include <boost/thread.hpp>
#include <boost/timer/timer.hpp>
#include "LockingQueue.hh"
#include "LockFreeQueue.hh"

fys::mq::LockingQueue<fys::mq::QueueContainer<std::string> > lockingQueue(5000);
fys::mq::LockFreeQueue<fys::mq::QueueContainer<std::string>, 5000> lockFreeQueue;

void addinlockingqueue(const fys::mq::QueueContainer<std::string> &container) {

    for (int i = 0; i < 1500; ++i) {
        lockingQueue.push(container);
    }
}

void addinlockfreequeue(const fys::mq::QueueContainer<std::string> &container) {

    for (int i = 0; i < 1500; ++i) {
        lockFreeQueue.push(container);
    }
}

void readLockFreeQueue() {
    int data = 0;

    for (int i = 0; data < 4500; ++i) {
        fys::mq::QueueContainer<std::string> *container;

        container = lockFreeQueue.pop();
        if (container) {
            std::cout<< "poped data n." << ++data << " value " << container->getContained() << std::endl;
        }
    }
}

void test1() {
    fys::mq::QueueContainer<std::string> c1("A");
    fys::mq::QueueContainer<std::string> c2("B");
    fys::mq::QueueContainer<std::string> c3("C");

    boost::thread w1(boost::bind(addinlockingqueue, c1));
    boost::thread w2(boost::bind(addinlockingqueue, c2));
    boost::thread w3(boost::bind(addinlockingqueue, c3));

    w1.join();
    w2.join();
    w3.join();
}


void test2() {
    fys::mq::QueueContainer<std::string> c1("A");
    fys::mq::QueueContainer<std::string> c2("B");
    fys::mq::QueueContainer<std::string> c3("C");

    boost::thread w1(boost::bind(addinlockfreequeue, c1));
    boost::thread w2(boost::bind(addinlockfreequeue, c2));
    boost::thread w3(boost::bind(addinlockfreequeue, c3));

    w1.join();
    w2.join();
    w3.join();
}

void test3() {
    fys::mq::QueueContainer<std::string> c1("A");
    fys::mq::QueueContainer<std::string> c2("B");
    fys::mq::QueueContainer<std::string> c3("C");

    boost::thread workerRead(readLockFreeQueue);
    boost::thread w1(boost::bind(addinlockfreequeue, c1));


    w1.join();
    sleep(2);
    boost::thread w2(boost::bind(addinlockfreequeue, c2));
    boost::thread w3(boost::bind(addinlockfreequeue, c3));
    w2.join();
    w3.join();
    workerRead.join();
}

int main(void)
{
    boost::timer::cpu_timer cpu_timer;
    test3();
    std::cout << "timer : " << cpu_timer.format() << std::endl;
    return 0;
}
