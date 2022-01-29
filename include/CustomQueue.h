#pragma once

#include <queue>
#include <vector>
#include <string>
#include <mutex>

/// <summary>
/// This is a Templated Custom Queue derived from vector. This has been made thread safe
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class CustomQueue
{
public:
	static constexpr uint32_t s_kmaxSymbols = 100000;

	explicit CustomQueue();
	void Push(const T &value);
	const T& Front();
	void Pop();
	bool IsDataFeedAvailable();

protected:
	std::vector<T> m_associatedData;

private:
	std::mutex m_mutex;
};

template <typename T>
CustomQueue<T>::CustomQueue() :
	m_associatedData()
{
	//reserve the size to compensate for the performance costs during runtime resizing
	m_associatedData.reserve(s_kmaxSymbols);
}

/// <summary>
/// Push the market data feed in Queue. Most probably this should be called by a DataSource
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="incomingMarketData"></param>
template <typename T>
void CustomQueue<T>::Push(const T &incomingMarketData)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	//do we have the symbol already
	auto it = std::find_if(m_associatedData.begin(), m_associatedData.end(),
		[&incomingMarketData](const T &queuedData) 
		{
			return incomingMarketData._symbol == queuedData._symbol;
		});

	//if not, then add in the symbols vector
	if (it == m_associatedData.end())
	{
		m_associatedData.push_back(incomingMarketData);
	}
	else
	{
		//if found, just overwrite the values with latest one
		it->_symbol = incomingMarketData._symbol;
		it->_price = incomingMarketData._price;
		it->_qty = incomingMarketData._qty;
	}
}

/// <summary>
/// Retrieves the top most data from the queue. Most probably this should be called by a Datasink
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns></returns>
template <typename T>
void CustomQueue<T>::Pop()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_associatedData.erase(m_associatedData.begin());
}

/// <summary>
/// Check if the Data feed is available in the Queue. Must be called by DataSink before reading the Queue
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns></returns>
template<typename T>
inline bool CustomQueue<T>::IsDataFeedAvailable()
{
	return !m_associatedData.empty();
}

/// <summary>
/// Just visits the data in the Queue without erasing or popping it off
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns></returns>
template <typename T>
const T& CustomQueue<T>::Front()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_associatedData.front();
}