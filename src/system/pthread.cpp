#include "catch.hpp"
#include "../utils/utils.h"
#include <pthread.h>
#include <set>
#include <stdexcept>

namespace System
{
namespace{

template <typename T>
class ThreadLocalPtr {
public:
    ThreadLocalPtr(T *address = 0) {
        if (pthread_key_create(&m_valueKey, 0)) {
            throw std::runtime_error("pthread_key_create failed");
        }
        if (address) {
            reset(address);
        }
    }

    ~ThreadLocalPtr() {
        pthread_key_delete(m_valueKey);
    }

    /**
     * Returns the raw pointer.
     */
    T *get() {
        return reinterpret_cast<T*>(::pthread_getspecific(m_valueKey));
    }

    T& operator*() {
        return *get();
    }

    T *operator->() {
        return get();
    }

    T *release() {
        T *value = get();
        reset(0);
        return value;
    }

    void reset(T *address) {
        pthread_setspecific(m_valueKey, address);
    }

private:
    ThreadLocalPtr(const ThreadLocalPtr& other);
    ThreadLocalPtr& operator=(const ThreadLocalPtr& other);
    ::pthread_key_t m_valueKey;
};

class Test{
    public:
        int id;
        char *p;
        Test(int i):id(i){
            p = new char;
            LOG << "Thread ID[" << pthread_self() << "] Test(" << id << "),"<<p <<std::endl;
        }
        ~Test(){
            LOG << "Thread ID[" << pthread_self() << "] ~Test(" << id << "),"<<p<<std::endl;
            delete p;
        }
        void report(){
            LOG << "Thread ID[" << pthread_self() << "] Test report (" << id << "),"<<p<<std::endl;
        }
};

Test t(1);
ThreadLocalPtr<Test> local_t(&t);

void *myThreadFun(void *vargp){
    LOG << "Thread ID[" << pthread_self() << "] started"<<std::endl;
    t.report();
    LOG << "Thread ID[" << pthread_self() << "] ended"<<std::endl;
    return 0;
}

}

TEST_CASE("pthread thread local", "[system][pthread]") {
    pthread_t t1,t2;
    pthread_create(&t1, NULL, myThreadFun, NULL);
    pthread_create(&t2, NULL, myThreadFun, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}


}
