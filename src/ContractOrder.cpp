#include "ContractOrder.h"
#include "spdlog/spdlog.h"
// 合约订单类成员函数实现
// 计算浮动盈亏
double ContractOrder::calculatePnL()
{
    if (marginCurrency == symbol)
        { // 币本位盈亏计算
            if (direction == "long")
                {
                    // 多仓收益 = 面值 * |张数| * 合约乘数 *（1／开仓均价 - 1／标记价格）
                    // 使用 面值 * |张数| * 合约乘数 的USDT，买了多少币。等到卖出时，只需要卖出币，还 面值 * |张数| * 合约乘数 的USDT即可，剩余都是利润。
                    return faceValue * quantity * multiplier * (1 / openPrice - 1 / markPrice);
                }
            else if (direction == "short")
                {
                    // 空仓收益 = 面值 * |张数| * 合约乘数 *（1／标记价格 - 1／开仓均价）
                    return faceValue * quantity * multiplier * (1 / markPrice - 1 / openPrice);
                }
            else
                {
                    spdlog::error("交易方向错误");
                    return 0.0;
                }
        }
    else
        { // USDT保证金盈亏计算
            if (direction == "long")
                {
                    // 面值 * |张数| * 合约乘数 *（标记价格 - 开仓均价）
                    return faceValue * quantity * multiplier * (markPrice - openPrice);
                }
            else if (direction == "short")
                {
                    // 面值 * |张数| * 合约乘数 *（开仓均价 - 标记价格）
                    return faceValue * quantity * multiplier * (openPrice - markPrice);
                }
            else
                {
                    spdlog::error("交易方向错误");
                    return 0.0;
                }
        }
};

// 平仓函数，返回实现盈亏
double ContractOrder::close()
{
    double pnl = calculatePnL();
    double realizedPnL = pnl - cost + marginAmount; // 卖出时再收取一次手续费
    return realizedPnL;
}

void ContractOrder::updatePrice(double price)
{
    markPrice = price;
}
