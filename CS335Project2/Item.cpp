#include "Item.hpp"

Item::Item(const std::string& name, const float& weight, const ItemType& type)
    : name_{ name }, weight_{ weight }, type_{ type } {}

bool Item::operator==(const Item& rhs) const {
    return name_ == rhs.name_;
}

std::ostream& operator<<(std::ostream& os, const Item& item) {
    os << item.name_;
    return os;
}

size_t std::hash<Item>::operator()(const Item& i) const {
    return std::hash<std::string>{}(i.name_);
}
