#include "HashInventory.hpp"
#include "Compare.hpp"

template <class Comparator>
Inventory<Comparator, std::unordered_set<Item>>::Inventory()
{
    weight_ = 0.0;
    equipped_ = nullptr;
}

template <class Comparator>
Item* Inventory<Comparator, std::unordered_set<Item>>::getEquipped() const
{
    return equipped_;
}

template <class Comparator>
void Inventory<Comparator, std::unordered_set<Item>>::equip(Item* itemToEquip)
{
    equipped_ = itemToEquip;
}

template <class Comparator>
void Inventory<Comparator, std::unordered_set<Item>>::discardEquipped()
{
    if (equipped_ != nullptr)
    {
        delete equipped_;
        equipped_ = nullptr;
    }
}

template <class Comparator>
float Inventory<Comparator, std::unordered_set<Item>>::getWeight() const
{
    return weight_;
}

template <class Comparator>
size_t Inventory<Comparator, std::unordered_set<Item>>::size() const
{
    return items_.size();
}

template <class Comparator>
std::unordered_set<Item> Inventory<Comparator, std::unordered_set<Item>>::getItems() const
{
    return items_;
}

template <class Comparator>
bool Inventory<Comparator, std::unordered_set<Item>>::pickup(const Item& target)
{
    if (items_.find(target) != items_.end())
    {
        return false;
    }
    items_.insert(target);
    weight_ += target.weight_;
    return true;
}

template <class Comparator>
bool Inventory<Comparator, std::unordered_set<Item>>::discard(const std::string& itemName)
{
    for (auto it = items_.begin(); it != items_.end(); ++it)
    {
        if (it->name_ == itemName)
        {
            weight_ -= it->weight_;
            items_.erase(it);
            return true;
        }
    }
    return false;
}

template <class Comparator>
bool Inventory<Comparator, std::unordered_set<Item>>::contains(const std::string& itemName) const
{
    return items_.find(Item(itemName)) != items_.end();
}

template <class Comparator>
std::unordered_set<Item> Inventory<Comparator, std::unordered_set<Item>>::query(const Item& start, const Item& end) const
{
    std::unordered_set<Item> result;

    if (Comparator::lessThan(end, start)) return result;

    for (const Item& item : items_)
    {
        if (Comparator::leq(start, item) && Comparator::leq(item, end))
        {
            result.insert(item);
        }
    }

    return result;
}

template <class Comparator>
Inventory<Comparator, std::unordered_set<Item>>::~Inventory()
{
    if (equipped_ != nullptr)
    {
        delete equipped_;
        equipped_ = nullptr;
    }
}
