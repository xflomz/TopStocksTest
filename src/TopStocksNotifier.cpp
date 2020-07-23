#include "TopStocksNotifier.h"

#include "TopStocksNotifierImpl.h"

namespace StockStuff {


TopStocksNotifier::SharedPtr TopStocksNotifier::Create(
    Implementation impl) {

    switch (impl) {
        case Implementation::TRIVIAL:
            return std::make_shared<TopStocksNotifierTrivial>();
        case Implementation::THREADED:
            return nullptr;
        default:
            return nullptr;
    }

}



} // namespace StockStuff