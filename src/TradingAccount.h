#ifndef TRADINGACCOUNT_H
#define TRADINGACCOUNT_H

#include <vector>
#include <string>
#include <unordered_map>

#include "SpotOrder.h"
#include "ContractOrder.h"

class TradingAccount
{
private:
    std::unordered_map<std::string, double> spotPrice; // 现货币种对USDT的价格
    std::vector<SpotOrder *> spotOrders;               // 现货交易记录
    std::vector<ContractOrder *> contractOrders;       // 合约交易记录
    std::vector<ContractOrder *> openContracts;        // 未平仓合约

public:
    std::unordered_map<std::string, double> holdings; // 各币种持仓

    // 构造函数，接受一个unordered_map来初始化各币种持仓
    TradingAccount(const std::unordered_map<std::string, double> &initialHoldings) :
        holdings(initialHoldings)
    {
        spotPrice["USDT"] = 1.0;
    }

    // 添加现货交易
    void addSpotOrder(SpotOrder *order);

    // 添加合约交易
    void addContractOrder(ContractOrder *order);

    // 平仓合约
    void closeContractOrder(ContractOrder *order);

    // 获取持有币种数量
    double getHoldingAmount(const std::string &currency);

    // 获取现货币种对USDT的价格
    double getSpotPrice(const std::string &currency);

    // 计算当前净值
    double calculateNetValue();

    // 处理现货交易后的币种更新，在addSpotOrder中调用
    void updateHoldingsForSpotOrder(SpotOrder *order);

    // 更新现货币种对USDT的价格
    void updateSpotPrice(const std::string &currency, double price);

    // 更新合约交易的价格
    void updateContractPrice(const std::string &currency, double price);

    // 获取所有未平仓合约
    std::vector<ContractOrder *> getOpenContracts();
};

#endif // TRADINGACCOUNT_H