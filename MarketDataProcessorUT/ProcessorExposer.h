#pragma once

#include "../include/Processor.h"
#include "../include/MarketData.h"


class ProcessorExposer : public Processor
{
public:

    ProcessorExposer() : Processor() {}

    virtual ~ProcessorExposer() {};

    std::vector<std::shared_ptr<MarketData>> GetAccumulatedSymbolsData() { return m_accumulatedSymbolsData; }

    void Process(const MarketData& queueElement) { Processor::Process(queueElement); }

    std::array<std::shared_ptr<MarketData>, 10> GetTop10Symbols()
    {
        return Processor::GetTop10Symbols();
    }
};