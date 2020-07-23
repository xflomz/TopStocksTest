#include <memory>
#include <vector>

namespace StockStuff {

class TopStocksNotifier {
public:
    using SharedPtr = std::shared_ptr<TopStocksNotifier>;
    using WeakPtr = std::weak_ptr<TopStocksNotifier>;

    enum class Implementation {
        TRIVIAL,
        THREADED
    };

    struct StockEntry {
        int id{0};
        double first_price{0.0f};
        double last_price{0.0f};
        double change_percent;
    }

    class Listener {
    public:
        using SharedPtr = std::shared_ptr<Listener>;
        using WeakPtr = std::weak_ptr<Listener>;

    public:
        virtual void TopGainersChanged(const std::vector<StockEntry> &) = 0;
        virtual void TopLosersChanged(const std::vector<StockEntry> &) = 0;
        
        // TODO: add error callback

        virtual ~Listener() = default;
    };

public:
    static SharedPtr Create(Implementation);
    
    virtual ~TopStocksNotifier() = default;

    virtual void OnQuote(int stock_id, double price) = 0;
    virtual void Subscribe(Listener::WeakPtr) = 0;
    virtual void Unsubscribe(Listener::WeakPtr) = 0;
};





} // namespace StockStuff