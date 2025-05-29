#include <iostream>
#include <chrono>
#include <vector>
#include <list>
#include <unordered_set>
#include <iomanip>

#include "Item.hpp"
#include "Compare.hpp"
#include "ItemGenerator.hpp"
#include "Inventory.hpp"
#include "HashInventory.hpp"
#include "TreeInventory.hpp"

using namespace std;
using namespace std::chrono;

template <typename Comparator, typename Container>
float benchmarkContains(size_t n, const string& label) {
    Inventory<Comparator, Container> inv;
    ItemGenerator gen(42);
    vector<string> contained, missing;

   
    for (size_t i = 0; i < n; ++i) {
        inv.pickup(gen.randomItem());
    }

    
    for (int i = 0; i < 100; ++i) {
        contained.push_back(gen.randomUsedName());
    }

   
    for (int i = 0; i < 100; ++i) {
        string fake = gen.randomItem().name_;
        missing.push_back(fake);
    }

    auto start = high_resolution_clock::now();
    for (const auto& name : contained) inv.contains(name);
    for (const auto& name : missing) inv.contains(name);
    auto end = high_resolution_clock::now();

    duration<float, milli> elapsed = end - start;
    float avgTime = elapsed.count() / 200.0f;

    cout << label << " | n = " << setw(5) << n << " | contains avg: " << fixed << setprecision(4) << avgTime << " ms" << endl;
    return avgTime;
}

template <typename Comparator, typename Container>
float benchmarkQuery(size_t n, const string& label, bool byWeight) {
    Inventory<Comparator, Container> inv;
    ItemGenerator gen(42);

    
    for (size_t i = 0; i < n; ++i) {
        inv.pickup(gen.randomItem());
    }

    auto start = high_resolution_clock::now();
    for (int i = 0; i < 10; ++i) {
        if (byWeight) {
            float w1 = gen.randomFloat(ItemGenerator::MIN_WEIGHT, ItemGenerator::MAX_WEIGHT - 0.2);
            Item low("low", w1, ItemType::NONE);
            Item high("high", w1 + 0.1, ItemType::NONE);
            inv.query(low, high);
        }
        else {
            string name1 = gen.randomUsedName();
            string name2 = gen.randomUsedName();
            if (name1 > name2) swap(name1, name2);
            inv.query(Item(name1, 0, ItemType::NONE), Item(name2, 0, ItemType::NONE));
        }
    }
    auto end = high_resolution_clock::now();
    duration<float, milli> elapsed = end - start;
    float avgTime = elapsed.count() / 10.0f;

    cout << label << " | n = " << setw(5) << n << " | query (" << (byWeight ? "weight" : "name") << ") avg: " << fixed << setprecision(4) << avgTime << " ms" << endl;
    return avgTime;
}

int main() {
    vector<size_t> sizes = { 1000, 2000, 4000, 8000 };

    cout << "----- Part A: contains() -----" << endl;
    for (size_t n : sizes) {
        benchmarkContains<CompareItemName, vector<Item>>(n, "Vector");
        benchmarkContains<CompareItemName, list<Item>>(n, "List");
        benchmarkContains<CompareItemName, unordered_set<Item>>(n, "Hash");
        benchmarkContains<CompareItemName, Tree>(n, "Tree");
    }

    cout << "\n----- Part B: query() by name -----" << endl;
    for (size_t n : sizes) {
        benchmarkQuery<CompareItemName, vector<Item>>(n, "Vector", false);
        benchmarkQuery<CompareItemName, list<Item>>(n, "List", false);
        benchmarkQuery<CompareItemName, unordered_set<Item>>(n, "Hash", false);
        benchmarkQuery<CompareItemName, Tree>(n, "Tree", false);
    }

    cout << "\n----- Part B: query() by weight -----" << endl;
    for (size_t n : sizes) {
        benchmarkQuery<CompareItemWeight, vector<Item>>(n, "Vector", true);
        benchmarkQuery<CompareItemWeight, list<Item>>(n, "List", true);
        benchmarkQuery<CompareItemWeight, unordered_set<Item>>(n, "Hash", true);
        benchmarkQuery<CompareItemWeight, Tree>(n, "Tree", true);
    }

    return 0;
}
