#include "TradingAccount.h"
#include "SpotOrder.h"
#include "ContractOrder.h"
#include "Strategy.h"
#include "spdlog/spdlog.h"
int main()
{
    spdlog::set_level(spdlog::level::info);
    // 创建交易账户对象
    std::unordered_map<std::string, double> initialHoldings = {
        {"USDT", 40000.0}};
    TradingAccount account(initialHoldings);

    class SimpleArbitrage : public FS_Arbitrage
    {
    public:
        SimpleArbitrage(TradingAccount *account) :
            FS_Arbitrage(account)
        {
        }

        // 实现虚函数 execute
        void execute() override
        {
            // 创建现货订单并添加到账户
            SpotOrder *spotOrder = new SpotOrder("BTC", "USDT", 0.00483908, 104352.2, 1e-3, "buy");
            account->addSpotOrder(spotOrder);

            // 创建合约订单并添加到账户
            ContractOrder *contractOrder = new ContractOrder("BTC", 108185.9, 1, 5, 5e-4, "short", "BTC");
            account->addContractOrder(contractOrder);

            SpotOrder *spotOrder2 = new SpotOrder("USDT", "BTC", 0.00021272, 1.0 / 104340.1, 1e-3, "sell");
            account->addSpotOrder(spotOrder2);

            initialCapital = 500.0; //! 需要手动设置初始资金
            //! 需要手动修改合约面值
        }
    };
    // 创建套利策略对象

    SimpleArbitrage strategy(&account);
    // 执行套利策略
    strategy.execute();
    double mkp = 100000;
    // 计算收益率
    double profitRate = strategy.calProfitRate("BTC", mkp);
    spdlog::info("Profit Rate: {:.4f}%", profitRate * 100);

    // double profitRate2 = strategy.calProfitRate("BTC", 2.3328, "BTC", 2.3511);
    // spdlog::info("Profit Rate at Now: {:.4f}%", profitRate2 * 100);
    // 计算弹性
    double profitElasticity = strategy.calProfitElasticity("BTC", mkp);
    spdlog::info("Profit Elasticity: {:.4f} BP", profitElasticity);

    // 计算对冲数量
    double hedgeAmount = strategy.calHedgeAmount();
    spdlog::info("Hedge Amount in BTC: {:.4f}", hedgeAmount);
    spdlog::info("Hedge Amount in USDT: {:.4f}", hedgeAmount * mkp);

    return 0;
}