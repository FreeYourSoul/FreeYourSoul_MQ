//
// Created by FyS on 20/04/17.
//

#ifndef FREESOULS_LOCKFREEQUEUE_HH
#define FREESOULS_LOCKFREEQUEUE_HH

#include <array>
#include <iostream>
#include "QueueContainer.hh"

namespace fys {
    namespace mq {

        template <typename TypeContainer, int SizeLFQ>
        class LockFreeQueue {

        public:
            ~LockFreeQueue() {}
            LockFreeQueue() : _tail(0), _maxReadTail(0), _head(0) {}
            LockFreeQueue(const LockFreeQueue& other) {
                this->_tail = 0;
                this->_head = 0;
                this->_maxReadTail = 0;
            }

            TypeContainer *pop() {
                u_int currentReadTail = getIndex(_maxReadTail.load(std::memory_order_relaxed));
                u_int currentHead = getIndex(_head);

                if (currentHead < currentReadTail)
                    return &_queue[_head++];
                return NULL;
            }

            void push(const TypeContainer &elem) {
                u_int currentTail = _tail.fetch_add(1, std::memory_order_relaxed);
                u_int indexNewElem = getIndex(currentTail + 1);
                u_int tstValue;

                _queue[indexNewElem] = elem;
                do {
                    tstValue = currentTail;
                } while (!_maxReadTail.compare_exchange_weak(tstValue, currentTail + 1));

            }

        private:
            inline u_int getIndex(const u_int index) const {
                return index % SizeLFQ;
            }

        private:
            std::atomic_uint _tail;
            std::atomic_uint _maxReadTail;
            u_int            _head;

            std::array<TypeContainer, SizeLFQ> _queue;
        };

    }
}

#endif //FREESOULS_LOCKFREEQUEUE_HH
