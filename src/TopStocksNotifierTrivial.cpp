#include "TopStocksNotifierTrivial.h"

#include <limits>

static constexpr int TOP_STOCKS_LIST_SIZE = 10;

namespace StockStuff {

TopStocksNotifierTrivial::TopStocksNotifierTrivial(TopStocksNotifier::Listener::WeakPtr wlistener)
    : wlistener_(wlistener) {
}

TopStocksNotifierTrivial::~TopStocksNotifierTrivial() {
}

void TopStocksNotifierTrivial::OnQuote(int stock_id, double price) {
    if (price <= std::numeric_limits<double>::epsilon()) {
        return;
    }

    bool is_new_entry = false;
    StockEntryInternal &stock = stocks_[stock_id];
    if (stock.stock.first_price == 0.0f) {
        stock.stock.first_price = price;
        is_new_entry = true;
    } else if (stock.stock.last_price == price) {
        return;
    }

    double prev_change_percent = stock.stock.change_percent;
    stock.stock.change_percent = 100.0f * price / stock.stockk.first_price - 100;
    stock.stock.last_price = price;

    if (is_new_entry) {
        stock.index_it = price_index_.insert({stock.stock.change_percent, stock.stock});

        if (stock.stock.change_percent > gainers_bound_) {
            GainerChanged(stock);
        }

        if (stock.stock.change_percent < losers_bound_) {
            LoserChanged(stock);
        }
    } else {
        auto handle = price_index_.extract(stock.index_it);
        stock.index_it = price_index_.insert({stock.stock.change_percent, stock.stock});

        if (stock.stock.change_percent >= gainers_bound_) {
            GainerChanged(stock);
        }

        if (stock.stock.change_percent <= losers_bound_) {
            LoserChanged(stock);
        }
    }
}

void TopStocksNotifierTrivial::GainerChanged(StockEntryInternal &stock) {
    bool top_changed = false;
    int current_pos = 0;

    auto index_it = price_index_.rbegin();
    auto index_end = price_index_.rend();

    // Process old stocks
    for (; current_pos < gainers_.size() && index_it != index_end; ++current_pos, ++index_it) {
        if (gainers_[current_pos].id == stock.stock.id 
                && index_it->second.id == stock.stock.id) {
            // Stock is in top list and its position has not changed.
            return;
        }

        if (gainers_[current_pos].id != index_it->second.id) {
            gainers_[current_pos] = index_it->second;
            top_changed = true;
        }
    }

    if (current_pos < TOP_STOCKS_LIST_SIZE && index_it != index_end) {
        gainers_.push_back(index_it->second);
        top_changed = true;
    }

    if (!top_changed) {
        return;
    }

    gainers_bound_ = gainers_.back().change_percent;

    if (auto slistener = wlistener.lock()) {
        slistener->TopGainersChanged(gainers_);
    }
}

void TopStocksNotifierTrivial::LoserChanged(StockEntryInternal &stock) {
    bool top_changed = false;
    int current_pos = 0;

    auto index_it = price_index_.begin();
    auto index_end = price_index_.end();

    // Process old stocks
    for (; current_pos < gainers_.size() && index_it != index_end; ++current_pos, ++index_it) {
        if (losers_[current_pos].id == stock.stock.id 
                && index_it->second.id == stock.stock.id) {
            // Stock is in top list and its position has not changed.
            return;
        }

        if (losers_[current_pos].id != index_it->second.id) {
            losers_[current_pos] = index_it->second;
            top_changed = true;
        }
    }

    if (current_pos < TOP_STOCKS_LIST_SIZE && index_it != index_end) {
        losers_.push_back(index_it->second);
        top_changed = true;
    }

    if (!top_changed) {
        return;
    }

    losers_bound_ = losers_.back().change_percent;

    if (auto slistener = wlistener.lock()) {
        slistener->TopGainersChanged(gainers_);
    }
}

} // namespace StockStuff