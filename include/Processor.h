#pragma once

#include <algorithm>
#include <array>
#include <memory>
#include <vector>
#include <mutex>

/// <summary>
/// This Class processor incoming marketData and stores the objects in memory.
/// While storing the objects, it also accumulates only the qty per symbol
/// </summary>

struct MarketData;

class Processor
{
public:
	explicit Processor();
	virtual ~Processor();
	
	void Process(const MarketData& queueElement);
	std::array<std::shared_ptr<MarketData>, 10> GetTop10Symbols();

protected:
	std::vector<std::shared_ptr<MarketData>> m_accumulatedSymbolsData;

private:
	Processor(const Processor&) = delete;
	Processor(const Processor&&) = delete;
	Processor& operator=(const Processor&) = delete;
	Processor& operator=(const Processor&&) = delete;

	std::mutex m_mutex;
};

