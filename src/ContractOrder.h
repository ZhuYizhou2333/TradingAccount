#ifndef CONTRACT_ORDER_H
#define CONTRACT_ORDER_H

#include <string>

class ContractOrder
{
public:
    std::string symbol;         // 合约标的
    double faceValue;           // 合约面值(BTCUSD合约面值为100美元)
    double openPrice;           // 开仓价格
    double markPrice;           // 当前标记价格
    double leverage;            // 杠杆倍数
    int quantity;               // 合约张数
    double fee;                 // 手续费率
    double cost;                // 手续费成本
    double multiplier = 1;      // 合约乘数（BTCUSD合约乘数为1）
    double marginAmount;        // 保证金数量
    std::string marginCurrency; // 保证金币种
    std::string direction;      // 交易方向（"long" 或 "short"）
    double unrealizedPnL;       // 未实现盈亏

    ContractOrder(std::string sym, double openP, double lev, double quantity, double fee, std::string dir, std::string marginCurrency = "USDT") :
        symbol(sym), openPrice(openP), markPrice(openP), leverage(lev), quantity(quantity), direction(dir), marginCurrency(marginCurrency)
    {
        if (symbol == marginCurrency)
            {
                // 币本位保证金交易
                // 面值
                faceValue = 100; // todo 建立faceValue映射表
                //  初始保证金 = 面值 * |张数| * 合约乘数 / (开仓均价 * 杠杆倍数)
                marginAmount = faceValue * quantity * multiplier / (openPrice * leverage);
                cost = marginAmount * fee;
            }
        else
            {
                // USDT保证金交易
                // 面值
                faceValue = 0.01;
                // 保证金数量 = 面值 * |张数| * 合约乘数 * 开仓均价／杠杆倍数
                marginAmount = faceValue * quantity * multiplier * openPrice / leverage;
                cost = marginAmount * fee;
            }
    }

    // 计算浮动盈亏，更新标记价格时自动调用。币本位时返回盈利的币数量，USDT保证金时返回盈利的USDT数量
    double calculatePnL();

    // 平仓函数，返回实现盈亏，币本位时返回盈利的币数量，USDT保证金时返回盈利的USDT数量
    double close();

    // 更新标记价格
    void updatePrice(double price);
};

#endif // CONTRACT_ORDER_H