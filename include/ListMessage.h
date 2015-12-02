#pragma once

#include <queue>
#include <boost\\thread\\mutex.hpp>


template <class T>
class CMessageQueue
{
public:
	CMessageQueue()
	{

	}

	~CMessageQueue()
	{
	}

	CMessageQueue(const CMessageQueue& msgQueue)
	{
		m_queue = msgQueue.m_queue;
		m_mutex = msgQueue.m_mutex;
	}

	CMessageQueue& operator = (const CMessageQueue& msgQueue)
	{
		m_queue = msgQueue.m_queue;
		m_mutex = msgQueue.m_mutex;

		return *this;
	}

	int size()
	{
		return m_queue.size();
	}

	bool IsEmpty()
	{
		return m_queue.empty();
	}

	void Push(T& a)
	{
		m_mutex.lock();
		m_queue.push(a);
		m_mutex.unlock();
	}

	T Pop()
	{
		m_mutex.lock();
		T temp = m_queue.front();
		m_queue.pop();
		m_mutex.unlock();
		return temp;
	}

	void Clean()
	{
		m_mutex.lock();
		while (m_queue.size() > 0)
		{
			m_queue.pop();
		}

		m_mutex.unlock();
	}

private:
	std::queue<T> m_queue;
	boost::mutex m_mutex;
};

