#include "Strategy.h"
#include <string>
#include "spdlog/spdlog.h"
double FS_Arbitrage::calProfitElasticity(std::string currency, double finalPrice)
{
    // 根据finalPrice计算套利交易对的期末净值
    account->updateSpotPrice(currency, finalPrice);
    account->updateContractPrice(currency, finalPrice);
    double finalNetValue = account->calculateNetValue();
    account->updateSpotPrice(currency, finalPrice * (1 + 1e-4));
    account->updateContractPrice(currency, finalPrice * (1 + 1e-4));
    double finalNetValue2 = account->calculateNetValue();
    return (finalNetValue2 - finalNetValue) / initialCapital * 10000;
}

double FS_Arbitrage::calProfitRate(std::string currency, double finalPrice)
{
    // 根据finalPrice计算套利交易对的期末净值
    account->updateSpotPrice(currency, finalPrice);
    account->updateContractPrice(currency, finalPrice);
    double finalNetValue = account->calculateNetValue() - beginTotalAsset;

    return finalNetValue / initialCapital;
}

// 重载，根据两腿的价格计算套利交易对的期末净值
double FS_Arbitrage::calProfitRate(std::string spotcurrency, double spotPrice, std::string futureCurrency, double futurePrice)
{
    // 根据finalPrice计算套利交易对的期末净值
    account->updateSpotPrice(spotcurrency, spotPrice);
    account->updateContractPrice(futureCurrency, futurePrice);
    double finalNetValue = account->calculateNetValue() - beginTotalAsset;

    return finalNetValue / initialCapital;
}

double FS_Arbitrage::calHedgeAmount()
{
    // 计算为了保持风险中性，需要额外购买的现货币种数量（负数代表卖出）
    // 遍历所有未平仓合约
    double hedgeAmount = 0;
    std::vector<ContractOrder *> openContracts = account->getOpenContracts();
    for (auto contract : openContracts)
        {
            if (contract->marginCurrency == contract->symbol) // 币本位合约
                {
                    hedgeAmount += contract->faceValue * contract->quantity * contract->multiplier / contract->openPrice;
                    hedgeAmount -= contract->marginAmount;
                }
            else
                {
                    spdlog::error("USDT合约暂不支持对冲");
                }
        }
    // 减去现有持仓
    hedgeAmount -= account->getHoldingAmount("BTC"); // todo 应当为合约币种
    return hedgeAmount;
}
