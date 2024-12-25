#include <vector>
#include <string>
#include <unordered_map>

#include "TradingAccount.h"
#include "spdlog/spdlog.h"

// 交易账户类成员函数实现

void TradingAccount::addSpotOrder(SpotOrder *order)
{
    // 更新持仓
    spdlog::debug("Adding spot order: buy {} {} sell {} {}", order->buyAmount, order->buyCurrency, order->sellAmount, order->sellCurrency);
    updateHoldingsForSpotOrder(order);
    spotOrders.push_back(order);
}

void TradingAccount::addContractOrder(ContractOrder *order)
{
    // 从现货中扣除保证金和手续费
    holdings[order->marginCurrency] -= order->marginAmount + order->cost;
    spdlog::debug("Adding contract order: symbol {} openPrice {} marginCurrency {} marginAmount {} direction {}", order->symbol, order->openPrice, order->marginCurrency, order->marginAmount, order->direction);
    // 更新未平仓合约
    openContracts.push_back(order);
    contractOrders.push_back(order);
}

void TradingAccount::closeContractOrder(ContractOrder *order)
{
    spdlog::debug("Closing contract order: symbol {} openPrice {} marginCurrency {} marginAmount {} direction {}", order->symbol, order->openPrice, order->marginCurrency, order->marginAmount, order->direction);
    // 从未平仓合约中移除
    openContracts.erase(std::remove(openContracts.begin(), openContracts.end(), order), openContracts.end());
    // 更新持仓
    holdings[order->marginCurrency] += order->close(); // 平仓
}

double TradingAccount::getHoldingAmount(const std::string &currency)
{
    return holdings[currency];
}

double TradingAccount::getSpotPrice(const std::string &currency)
{
    return spotPrice[currency];
}

double TradingAccount::calculateNetValue()
{
    double netValue = 0.0;

    // 计算现货资产净值
    for (const auto &holding : holdings)
        {
            std::string currency = holding.first;
            double amount = holding.second;
            double price = getSpotPrice(currency);
            netValue += amount * price;
        }

    // 计算合约资产净值
    for (auto &contract : openContracts)
        {
            netValue += contract->calculatePnL() * getSpotPrice(contract->marginCurrency); // 加盈利
            netValue += contract->marginAmount * getSpotPrice(contract->marginCurrency);   // 加保证金
        }

    return netValue;
}

void TradingAccount::updateHoldingsForSpotOrder(SpotOrder *order)
{
    // 更新现货持仓数量
    holdings[order->buyCurrency] += order->NetbuyAmount;
    spdlog::debug("Holdings Buy: {} {}", order->buyCurrency, holdings[order->buyCurrency]);
    holdings[order->sellCurrency] -= order->sellAmount;
    spdlog::debug("Holdings Sell: {} {}", order->sellCurrency, holdings[order->sellCurrency]);
}

void TradingAccount::updateSpotPrice(const std::string &currency, double price)
{
    spotPrice[currency] = price;
}

void TradingAccount::updateContractPrice(const std::string &currency, double price)
{
    for (auto &contract : openContracts)
        {
            if (contract->symbol == currency)
                {
                    contract->updatePrice(price);
                }
        }
}

std::vector<ContractOrder *> TradingAccount::getOpenContracts()
{
    return openContracts;
}
