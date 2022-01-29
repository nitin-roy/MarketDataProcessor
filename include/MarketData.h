#pragma once

#include <string>

struct MarketData
{
	std::string _symbol;
	double _price;
	long long _qty;

	MarketData():
		_symbol(""),_price(0), _qty(0)
	{

	}

	MarketData(const std::string symbol, double price, unsigned long long qty):
		_symbol(symbol), _price(price), _qty(qty)
	{
	}
};