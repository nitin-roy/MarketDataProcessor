#include "pch.h"
#include "ProcessorExposer.h"

TEST(ProcessorTest, Process_Adds_New_MarketDataFeed)
{
	std::shared_ptr<ProcessorExposer> processor = std::make_shared<ProcessorExposer>();

	MarketData data1("ABC", 100, 200);

	processor->Process(data1);

	auto accumulatedData = processor->GetAccumulatedSymbolsData();

	ASSERT_EQ(accumulatedData.size(), 1);
	ASSERT_EQ(accumulatedData[0]->_symbol, "ABC");
	ASSERT_EQ(accumulatedData[0]->_price, 100);
	ASSERT_EQ(accumulatedData[0]->_qty, 200);
}

TEST(ProcessorTest, Process_Adds_Qty_If_MarketData_Exists)
{
	std::shared_ptr<ProcessorExposer> processor = std::make_shared<ProcessorExposer>();

	MarketData data1("ABC", 100, 200);
	processor->Process(data1);

	MarketData data2("ABC", 100, 100);
	processor->Process(data2);

	auto accumulatedData = processor->GetAccumulatedSymbolsData();

	ASSERT_EQ(accumulatedData.size(), 1);
	ASSERT_EQ(accumulatedData[0]->_symbol, "ABC");
	ASSERT_EQ(accumulatedData[0]->_price, 100);
	ASSERT_EQ(accumulatedData[0]->_qty, 300);
}

TEST(ProcessorTest, GetTop10Symbols_Returns_Less_Than_10_If_Less_Symbols)
{
	std::shared_ptr<ProcessorExposer> processor = std::make_shared<ProcessorExposer>();

	MarketData data1("ABC", 100, 200);
	processor->Process(data1);

	MarketData data2("DEF", 100, 300);
	processor->Process(data2);

	MarketData data3("GHI", 100, 500);
	processor->Process(data3);

	auto topSymbols = processor->GetTop10Symbols();

	ASSERT_EQ(topSymbols[0]->_symbol, "GHI");
	ASSERT_EQ(topSymbols[1]->_symbol, "DEF");
	ASSERT_EQ(topSymbols[2]->_symbol, "ABC");
	ASSERT_EQ(topSymbols[3], nullptr);

}

TEST(ProcessorTest, GetTop10Symbols_Returns_Only_Top10_If_More_Symbols)
{
	std::shared_ptr<ProcessorExposer> processor = std::make_shared<ProcessorExposer>();

	MarketData data1("SYMBOL1", 100, 10);
	processor->Process(data1);

	MarketData data2("SYMBOL2", 100, 20);
	processor->Process(data2);

	MarketData data3("SYMBOL3", 100, 50);
	processor->Process(data3);

	MarketData data4("SYMBOL4", 100, 330);
	processor->Process(data4);

	MarketData data5("SYMBOL5", 100, 200);
	processor->Process(data5);

	MarketData data6("SYMBOL6", 100, 300);
	processor->Process(data6);

	MarketData data7("SYMBOL7", 100, 500);
	processor->Process(data7);

	MarketData data8("SYMBOL8", 100, 250);
	processor->Process(data8);

	MarketData data9("SYMBOL9", 100, 210);
	processor->Process(data9);

	MarketData data10("SYMBOL10", 100, 200);
	processor->Process(data10);

	MarketData data11("SYMBOL11", 100, 100);
	processor->Process(data11);

	MarketData data12("SYMBOL12", 100, 90);
	processor->Process(data12);

	MarketData data13("SYMBOL3", 100, 25);
	processor->Process(data13);

	auto topSymbols = processor->GetTop10Symbols();

	ASSERT_EQ(topSymbols[0]->_symbol, "SYMBOL7");
	ASSERT_EQ(topSymbols[0]->_qty, 500);
	ASSERT_EQ(topSymbols[9]->_symbol, "SYMBOL3");
	ASSERT_EQ(topSymbols[9]->_qty, 75);
}