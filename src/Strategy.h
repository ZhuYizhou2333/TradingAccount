#ifndef STRATEGY_H
#define STRATEGY_H

#include "TradingAccount.h"
#include <string>

class FS_Arbitrage
{
private:
public:
    TradingAccount *account;

    double initialCapital; // 初始投入资金

    double beginTotalAsset; // 账户初始总资产

    FS_Arbitrage(TradingAccount *account) :
        account(account)
    {
        beginTotalAsset = account->calculateNetValue();
    }
    // 建仓虚函数，继承时实现具体策略，并且为initialCapital赋值
    virtual void execute() = 0;
    // 计算套利交易对期末价格的弹性，即最终价格上涨1BP时收益率上涨的BP值
    double calProfitElasticity(std::string currency, double finalPrice);
    // 计算套利交易的收益率
    double calProfitRate(std::string currency, double finalPrice);
    // 重载，根据两腿的价格计算套利交易对的期末净值
    double calProfitRate(std::string spotcurrency, double spotPrice, std::string futureCurrency, double futurePrice);
    // 计算为了保持风险中性，需要额外购买的现货币种数量（负数代表卖出）
    double calHedgeAmount();
};

#endif // STRATEGY_H