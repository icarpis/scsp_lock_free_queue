#pragma once

#include <cstring>

template <typename T>
class scsp_lock_free_queue
{
public:
    explicit scsp_lock_free_queue(size_t queue_size)
        : m_queue_size(queue_size + 1), m_head(0), m_tail(0)
    {
        m_arr = new T[m_queue_size];
    }

    ~scsp_lock_free_queue()
    {
        delete[] m_arr;
    }

    bool enqueue(T item)
    {
        if (((m_tail + 1) % m_queue_size) == m_head)
        {
            return false;
        }

        memcpy_s(&m_arr[m_tail], sizeof(T), &item, sizeof(item));
        m_tail = (m_tail + 1) % m_queue_size;

        return true;
    }

    bool dequeue(T* item)
    {
        if ((m_head == m_tail) || !item)
        {
            return false; // queue is empty
        }

        memcpy_s(item, sizeof(*item), &m_arr[m_head], sizeof(T));
        m_head = (m_head + 1) % m_queue_size;

        return true;
    }

private:
    scsp_lock_free_queue(const scsp_lock_free_queue&);
    const scsp_lock_free_queue& operator=(const scsp_lock_free_queue&);

    size_t  m_queue_size;
    T* m_arr;
    size_t  m_head;
    size_t  m_tail;
};