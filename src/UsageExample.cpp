#include "../include/CustomQueue.h"
#include "../include/MarketData.h"
#include "../include/Processor.h"

int main()
{
    //create an instance of CustomQueue
	std::shared_ptr<CustomQueue<MarketData>> customQueue = std::make_shared<CustomQueue<MarketData>>();

    //create an instance of Processor
    std::shared_ptr<Processor> processor = std::make_shared<Processor>();

    MarketData feed("AAPL", 200, 100);
    processor->Process(feed);

    customQueue->Push({ "AAPL", 100.1, 200});
    processor->Process({ "AAPL", 100.1, 200 });

    customQueue->Push({ "MSFT", 144.3, 300 });
    processor->Process({ "MSFT", 144.3, 300 });

    customQueue->Push({ "GOOGL", 324.56, 230 });
    processor->Process({ "GOOGL", 324.56, 230 });

    customQueue->Push({ "ABC", 201, 101 });
    processor->Process({ "ABC", 201, 101 });

    auto top10Symbls = processor->GetTop10Symbols();

	return 0;

}