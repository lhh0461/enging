#include <stdio.h>
#include "MemPool.h"

struct test {
    int a;
    int b;
    int c;
    ~test()
    {
        printf("on ~test, a=%d,b=%d,c=%d\n", a, b, c);   
    }
};

int test()
{
    XEngine::CMemPool<struct test> *pool = new XEngine::CMemPool<struct test>(10);
    std::list<struct test*> l;
    for (int i = 0; i < 20; i++) {
        struct test *t = pool->Malloc();
        t->a = 10 + i;
        t->b = 10 + i;
        t->c = 10 + i;
        l.push_back(t);
        printf("t->a=%d,t->b=%d,t->c=%d\n", t->a, t->b, t->c);   
    }

    while (!l.empty())
    {
        struct test *p = l.front();
        l.pop_front();
        pool->Free(p);
    }

    for (int i = 0; i < 20; i++) {
        struct test *t = pool->Malloc();
        t->a = 10 + i;
        t->b = 10 + i;
        t->c = 10 + i;
        l.push_back(t);
        printf("again t->a=%d,t->b=%d,t->c=%d\n", t->a, t->b, t->c);   
    }

    delete pool;
}
