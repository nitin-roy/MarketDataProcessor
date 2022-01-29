
#include "Processor.h"
#include "MarketData.h"

static constexpr int s_kmaxSymbols = 100000;

Processor::Processor() :m_accumulatedSymbolsData()
{
	//reserving upfront to cut on re-initialization costs on performance, as and when symobls gets added
	m_accumulatedSymbolsData.reserve(s_kmaxSymbols);
}

Processor::~Processor()
{
	m_accumulatedSymbolsData.clear();
}

/// <summary>
/// This method is responsible to store the received market feed in memory and perform computations as
/// either to add a new entry in memory or just add the incoming qty to already stored qty for symbol 
/// </summary>
/// <param name="queueElement"></param>
void Processor::Process(const MarketData& queueElement)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	//check if the current symbol is already in memory
	auto it = std::find_if(m_accumulatedSymbolsData.begin(), m_accumulatedSymbolsData.end(),
		[&queueElement](const std::shared_ptr<MarketData>& accumulatedData)
		{
			return queueElement._symbol == accumulatedData->_symbol;
		});

	//if not, then add it in memory
	if (it == m_accumulatedSymbolsData.end())
	{
		m_accumulatedSymbolsData.push_back(std::make_shared<MarketData>(queueElement._symbol, queueElement._price, queueElement._qty));
	}
	else
	{
		//add the qty of symbol feed data
		//nothing is mentioned about the price in the problem statement	
		(*it)->_qty += queueElement._qty;
	}
}

/// <summary>
/// This method retrieves the top 10 symbols from the memory store and sorting is performed on demand to
/// avoid getting performance hit impacts if done in ProcessMarketDataFeed
/// </summary>
/// <returns>top 10 symbols from the market book</returns>
std::array<std::shared_ptr<MarketData>, 10> Processor::GetTop10Symbols()
{
	auto compareFunction = [](const std::shared_ptr<MarketData>& ldata, const std::shared_ptr<MarketData>& rdata)
	{
		return (ldata->_qty > rdata->_qty);
	};

	{
		std::lock_guard<std::mutex> lock(m_mutex);
		sort(m_accumulatedSymbolsData.begin(), m_accumulatedSymbolsData.end(), compareFunction);
	}

	std::array<std::shared_ptr<MarketData>, 10> topSymbolsQtywise = {};
	auto topElementsCount = (m_accumulatedSymbolsData.size() > 10) ? 10 : m_accumulatedSymbolsData.size();

	std::copy(m_accumulatedSymbolsData.begin(), m_accumulatedSymbolsData.begin() + topElementsCount, topSymbolsQtywise.begin());

	return topSymbolsQtywise;
}

