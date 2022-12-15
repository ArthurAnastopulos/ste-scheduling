#ifndef __OBSERVER_H__
#define __OBSERVER_H__


class Observer
{
public:
    virtual ~Observer() {};
    virtual void update(int val) {};
};


#endif
