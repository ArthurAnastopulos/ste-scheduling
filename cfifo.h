#ifndef __CFIFO_H__
#define __CFIFO_H__

template<int N, typename T>
class CFIFO
{
public:


    CFIFO()
    {
        for (int i = 0; i < N; i++)
            data[i] = 0;

        in = N-1;
    }

    ~CFIFO() {

    }

    void enqueue(T obj)
    {
        in = ((in == N-1) ? 0 : (in+1));
        data[in] = obj;
    }

    T get(int i)
    {
        int ii = ((unsigned)(in - i)) % N;
        return data[ii];
    }

    T average()
    {  
        int avg = 0;
        for (int i = 0; i < N; i++)
        avg = avg + data[i];

        return avg / N;
    }
  
private:
    T data[N];
    int in;
};


#endif /* __CFIFO_H__ */