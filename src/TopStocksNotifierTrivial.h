#include "TopStocksNotifier.h"

#include <unordered_map>
#include <map>

namespace StockStuff {

class TopStocksNotifierTrivial : public TopStocksNotifier {
public:
    TopStocksNotifierTrivial(TopStocksNotifier::Listener::WeakPtr);

    ~TopStocksNotifierTrivial() override;
    
    void OnQuote(int stock_id, double price) override;

private:
    struct StockEntryInternal {
        StockEntry stock;

        std::multimap<double, StockEntry &>::iterator index_it;
    };

private:
    void GainerChanged(StockEntryInternal &);
    void LoserChanged(StockEntryInternal &);

private:
    TopStocksNotifier::Listener::WeakPtr wlistener_;

    std::unordered_map<int, StockEntryInternal> stocks_;
    std::multimap<double, StockEntry &> price_index_;

    std::vector<StockEntry> gainers_;
    std::vector<StockEntry> losers_;

    double gainers_bound_{0.0f};
    double losers_bound_{0.0f};
}


} // namespace StockStuff