#ifndef SPOTORDER_H
#define SPOTORDER_H

#include <string>
// 现货订单类，
class SpotOrder
{
public:
    std::string buyCurrency;  // 买入币种
    std::string sellCurrency; // 卖出币种
    double buyAmount;         // 买入数量
    double sellAmount;        // 卖出数量
    double price;             // 价格 等于sellAmount/buyAmount
    double NetbuyAmount;      // 扣除手续费后的买入数量
    double fee;               // 手续费率

    // 构造函数：通过给定价格和买入数量下单，type为buy表示固定买入额度，为sell表示固定卖出额度。价格始终为卖出数量除以买入数量
    SpotOrder(std::string buy, std::string sell, double amt, double prc, double f, std::string type = "buy") :
        buyCurrency(buy), sellCurrency(sell), price(prc), fee(f)
    {
        if (type == "buy")
            { // 固定买入额度
                NetbuyAmount = amt;
                buyAmount = NetbuyAmount / (1 - fee);
                sellAmount = buyAmount * price;
            }
        else
            { // 固定卖出额度
                sellAmount = amt;
                buyAmount = sellAmount / price;
                NetbuyAmount = buyAmount * (1 - fee);
            }
    }
};

#endif // SPOTORDER_H
