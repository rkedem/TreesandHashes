# Project 2 - 

## Task 1: Hash Me If You Can!

### Part A: Defining a Custom Hash
Let's start off simple. 

Right now we don't have a hash for our `Item` object. Try creating an `unordered_set<Item>` and seeing what happens. Then, come back and continue reading. 

Nothing happens. An error occurs. What's going on? Because `unordered_set` is *hash-based,* and `Item` has no hash.

Let's fix that. You'll override `std::hash<Item>`.

Within `Item.hpp` and *outside* the `Item` class, define the following interface. 
```c++
template <>
struct std::hash<Item> {
    /**
     * @brief Computes a hash value for an Item based on
     * using the standard hash for strings on the Item's name
     *
     * @param i The Item to hash
     * @return Hash value for the Item
     */
    size_t operator()(const Item& i) const;
};
```

You'll notice the **template is empty.** Instead, we explicitly specify `std::hash` for `Item` using `std::hash<Item>`. This is called **template specialization.**

--- 
#### A Quick Guide on Template Specialization (skip if you already know about it)

**What this means is that when we have a templated class or function, we can provide a specific implementation for specific types. This allows us to override the general template behavior for certain cases while keeping the flexibility of templates for others.**

Consider the following example:
```c++
// General template
template <typename T>
class Printer {
public:
    void print(T value) {
        std::cout << "General: " << value << std::endl;
    }
};

// Specialization for `std::string`
template <>
class Printer<std::string> {
public:
    void print(std::string value) {
        std::cout << "Specialized for string: " << value << std::endl;
    }
};

int main() {
    Printer<int> intPrinter;
    intPrinter.print(42);  // Uses general template

    Printer<std::string> stringPrinter;
    stringPrinter.print("Hello!");  // Uses specialized template

    return 0;
}
```

Output:
```cmd
General: 42  
Specialized for string: Hello!
```

**Make sure you understand this before moving forward. The rest of the project relies on this.**

---

Now, let's finish up our specialized hash for `Item`. 

In `Item.cpp` you'll write the hash implementation, by relying on the `std::hash<std::string>` on the Item's `name_` member.
Refer to: https://en.cppreference.com/w/cpp/utility/hash for how to do so.
<small>Don't overcomplicate this--yes it's a one-liner.</small>

```c++
/**
 * @brief Computes a hash value for an Item based on
 * using the standard hash for strings on the Item's name
 *
 * @param i The Item to hash
 * @return Hash value for the Item
 */
size_t std::hash<Item>::operator()(const Item& i) const {
    // Your code here...
}
```

### Part B: Custom Comparators
Ok, so we can hash our Item, great. But how do we compare our items? What makes the `Hand of Malenia` greater than the `Straight Sword`?

Here's where custom comparators come in. We'll define multiple ways to compare our items, based on their properties; specifically, with respect to:
1) Lexicographical order of their names
2) Weight
3) Type

We've provided you with two of these classes already. In `Compare.cpp`, implement `CompareItemName` which acts as a wrapper for comparing the lexicographic order of two Item's names:

```c++
/**
 * @class CompareItemName
 * @brief Provides static comparison functions for Item names
 */
class CompareItemName {
public:
    /**
     * @brief Compares two items by name
     * @param a First item to compare
     * @param b Second item to compare
     * @return true if a's name is less than b's name
     */
    static bool lessThan(const Item& a, const Item& b);

    /**
     * @brief Checks if two items have equal names
     * @param a First item to compare
     * @param b Second item to compare
     * @return true if items have equal names
     */
    static bool equal(const Item& a, const Item& b);

    /**
     * @brief Checks if first item's name is less than or equal to second item's
     * name
     * @param a First item to compare
     * @param b Second item to compare
     * @return true if a's name is less than or equal to b's name
     */
    static bool leq(const Item& a, const Item& b);
};
```

#### Usage Example
Suppose you have two items:
```c++
Item katana("Katana", 5.5, ItemType::WEAPON);
Item sword("Sword", 5.2, ItemType::WEAPON);
```
To compare different properties of `katana` and `sword`:
```c++
CompareItemName::lessThan(katana, sword);   // True, 'katana' < 'sword'
CompareItemWeight::lessThan(katana, sword); // False, 5.5 not < 5.2
CompareItemType::equal(katana, sword);      // True, WEAPON == WEAPON
```

## Task 2: Ready? Set? Go!
### A Brief Description of `Inventory`
So now that we've gotten the Item functionalities down, let's move on to our Inventory class. In `Inventory.hpp` and `Inventory.cpp` we've provided a base implementation that uses a generic, iterable container under the hood (eg. `std::vector`, `std::list`). **`Inventory` objects only store items with unique names. No duplicate names allowed.** 

**Going forward, we want to specialize the `Inventory` class implementation using specific containers so that we can *optimize certain operations.*** 

<small>Notice that sometimes, the implementation of certain functions will be the same (eg. `getEquipped()` across `Inventory` and `Inventory<Comparator, std::unordered_set<Item>>`). This is just an artifact of template specialization.</small>

### Part A: Setting up a `HashInventory`
We've given you `HashInventory.hpp` and the corresponding signatures in `HashInventory.cpp`. Implement the functions to match the specs given. Considering we have *no duplicate item names*, do you see why we hashed off the Item name? 

<small>Hint: This should also clue you in on how fast specific operations should take!</small>

For convenience, here's the function signatures & descriptions, as they appear in `HashInventory.hpp`
```c++
/**
 * @brief Default constructor for the Inventory template class.
 *
 * Initializes an empty inventory with no items, no equipped item,
 * and zero total weight.
 *
 * @tparam Comparator The comparison class for querying items
 */
Inventory();

/**
 * @brief Retrieves the value stored in `equipped_`
 * @return The Item pointer stored in `equipped_`
 */
Item* getEquipped() const;

/**
 * @brief Equips a new item.
 * @param itemToEquip A pointer to the item to equip.
 * @post Updates `equipped` to the specified item
 * without deallocating the original.
 */
void equip(Item* itemToEquip);

/**
 * @brief Discards the currently equipped item.
 * @post Deallocates the item pointed to by `equipped`
 * and sets `equipped` to nullptr, if `equipped` is not nullptr already.
 */
void discardEquipped();

/**
 * @brief Retrieves the value stored in `weight_`
 * @return The float value stored in `weight_`
 */
float getWeight() const;

/**
 * @brief Retrieves the value stored in `item_count_`
 * @return The size_t value stored in `item_count_`
 */
size_t size() const;

/**
 * @brief Retrieves a copy of the container holding inventory items.
 *
 * @return Container of items in the inventory
 */
std::unordered_set<Item> getItems() const;

/**
 * @brief Attempts to add a new item to the inventory.
 *
 * @param target Item to be added to the inventory
 * @return true if the item was successfully added, false if an item
 *         with the same name already exists
 * @post Updates the weight_ member to reflect the new Item pickup
 */
bool pickup(const Item& target);

/**
 * @brief Attempts to remove an item from the inventory by name.
 *
 * @param name Name of the item to be removed
 * @return true if the item was successfully removed, false if the
 *         item was not found in the inventory
 * @post Updates the weight_ member to reflect removing the Item
 */
bool discard(const std::string& itemName);

/**
 * @brief Checks if an item with the given name exists in the inventory.
 *
 * @param name Name of the item to search for
 * @return true if the item exists in the inventory, false otherwise
 */
bool contains(const std::string& itemName) const;

/**
 * @brief Destructor for the Inventory class.
 * @post Deallocates any dynamically allocated resources.
 */
~Inventory();
```

### Part B: Querying the `Inventory`
Ok so we have our items in the inventory. Now we want to be able to select all items that match a specific range of values. That's what `query()` will do.

**Here's an example.**
Suppose we have an inventory. We're going to be repeatedly querying for items within a certain weight.

1) We instantiate an `Inventory<CompareItemWeight>`, specifying that our queries will occur with respect to the Item's weight. 
```c++
Inventory<CompareItemWeight> myInventory;
```

2) Now, since we want to query all items weighing between **0.4** and **10.9**, we create two Items that represent our lower and upper bounds.
```c++
Item start("LowWeightItem", 0.4, ItemType::None); 
Item end("HighWeightItem", 10.9, ItemType::None);
```

3) Finally, we can use `query()` like this, which--thanks to `CompareItemWeight` will take each Item and *compare them using their `weight_` member*.
```c++
std::unordered_set<Item> result = myInventory.query(start, end); 
```
The result now contains all items with weights between 0.4 and 10.9 (inclusive).

**That's how you'd use the function. Now it's your turn to implement it:**

```c++
/**
 * @brief Queries the inventory for items within a specified range.
 *
 * Returns a set of items that fall between the start and end items
 * according to the specified Comparator (inclusive on both ends)
 *
 * @param start An Item whose compared property is the lower bound of the query range
 * @param end An Item whose compared property is the upper bound of the query range
 * @return std::unordered_set of items within the specified range
 *
 * @note Returns an empty set if the end item is less than the start item
 * @example To select all Items with weights 0.4 to 10.9, we'd setup the class & parameters as such:
 *  - this Inventory object is of type Inventory<CompareItemWeight>
 *  - start = Item("some_name", 0.4, ItemType::None)
 *  - end = = Item("some_other_name", 10.9, ItemType::None)
 *
 */
std::unordered_set<Item> query(const Item& start, const Item& end) const;
```

## Task 3: Branching Out to New Methods
Now we'll be making a tree-based implementation of `Inventory`, but first we have some setup to do. 

### Part A: Traversing the `ItemAVL` 
`ItemAvl<Comparator>` is an AVL tree whose ordering property (you know, since it's a BST and all) **relies on the templated Comparator class.** For example, an ItemAVL<CompareItemWeight> will order items *based off their weight*.

However, before we insert an Item, **we need to check that our tree does not have any Items with the name of the new Item.** This means, we cannot rely on our ordering property. For now, we'd like to use *no* additional memory to track unique names, so we'll need to check *every* item in the AVL to see if there's a name match. 

**NOTE: This is a non-standard way of implementing an AVLTree, and the efficiency-penalty we incur is due to the constraints we're imposing upon the AVL's values and the problem we're trying to solve. That is to say, this is a non-standard implementation, so make sure you don't get your runtime complexities wrong.**

<small>By the way, yes, you can optimize name search for `ItemAVL<CompareItemName>`, but for simplicity's sake, don't worry about it.</small>

Anywho, implement the `contains()` method in `ItemAVL` using the public `contains()` as a wrapper for the private helper `contain()` method (this should clue you in to the algorithm used to do so). Note these are signatures taken from `ItemAVL.hpp`, which you already have access to.
```c++
private:
    /**
     * @brief Internal routine to check if a subtree contains an item with a given name
     *
     * @param name The name to check for
     * @param subroot The root of the subtree to be searched
     * @return True if a matching Item exists, false otherwise.
     */
    bool contains(const std::string& name, const Node* subroot) const;
public:
    /**
     * @brief Checks if the tree contains an item with a given name
     * @param name The name to check for
     * @return True if a matching Item exists, false otherwise.
     */
    bool contains(const std::string& name) const;
```

### Part B: Querying a `TreeInventory`
We had you implement all of the functions for `HashInventory`, so we won't make you do everything again (not much has genuinely changed--take a look at the code and you'll see).

**NOTE: When instantiating a tree-based Inventory, use `Inventory<Comparator, Tree>`, not `Inventory<Comparator, ItemAVL<Comparator>>`**

Let's just cut to chase. We have an AVL Tree whose ordering property is based on the `Comparator` we pass in as a template--which is exactly what what we use to select items in the `query()` function.
 
This means that unlike in `ItemAVL`'s `contains()`, method, we can efficiently search for items based on the property (eg. `name_`, `weight_`, or `type_`) being using for the `Comparator`'s functions.

So let's do that! 

**Here's some thoughts to guide you:**
Say we're at a node in the tree.

If this node's Item is below the minimum of the query range, do we really need to continue searching the left subtree? Remember, everything to the left *is less than the current Item.* 

Similarly, if we're above the maximum of the query range, do we really need to continue searching the right, if everything to the right is *greater than our current Item?*

**That's your hint. The rest is for you to figure out.** All I'll say is that your AVL query shouldn't be O(n). 

Here's the function signature, for convenience. You'll more than likely need a helper function as well, so we've included a space for you to put that.
```c++
public:
/**
 * @brief Queries the inventory for items within a specified range.
 *
 * Returns a set of items that fall between the start and end items
 * according to the specified Comparator (inclusive on both ends)
 *
 * @param start An Item whose compared property is the lower bound of the query range
 * @param end An Item whose compared property is the upper bound of the query range
 * @return std::unordered_set of items within the specified range
 *
 * @note Returns an empty set if the end item is less than the start item
 * @example To select all Items with weights 0.4 to 10.9, we'd setup the class & parameters as such:
 *  - this Inventory object is of type Inventory<CompareItemWeight>
 *  - start = Item("some_name", 0.4, ItemType::None)
 *  - end = = Item("some_other_name", 10.9, ItemType::None)
 *
 */
std::unordered_set<Item> query(const Item& start, const Item& end) const;

private:
// This is the helper; I won't be providing docs for this one! On you to engineer how you might use it :) 
void queryHelper(const Item& start, const Item& end, const Node* root, std::unordered_set<Item>& result) const;
```

## Task 4: Time to Time
Now that we've implemented `contains()` and `query()` for our `Inventory` class, it's time to analyze their performance across different data structures. Your task is to measure and compare the runtime of these operations on different inventory implementations:  
1. `Inventory<Comparator>` (Uses a **vector**)
2. `Inventory<Comparator, std::list>` (Uses a **linked-list**)
3. `Inventory<Comparator, std::unordered_set<Item>>`
4. `Inventory<Comparator, Tree>`

**Before you go, some words of advice:** It looks like a lot of timing and testing, but it's not. Remember, all these Inventories *have the same interface, and there's a LOT of repeated steps.* Make sure to create templated functions where you can. 

For example, you could define a function like:
```c++
template <Comparator, Container>
float timeQuery(Inventory<Comparator, Container> inv, Item start, item end);
```
So you don't have to retype the same code over and over again! It'll make your life easier.

### Part A: Timing `contains()`
Consider:
```
n = { 1000, 2000, 4000, 8000 }
```

For each `Inventory` type listed above (using `CompareItemName` as the `Comparator`) and each value of n:
1. Create a new `ItemGenerator` with seed 42.
    **a.** Create & insert **n randomly generated items** into the inventory using `ItemGenerator::randomItem()`
    **b.** Using `ItemGenerator::randomUsedName()` select **100 random item names** and store them in a vector called `contained`.
    **c.** Now *use the same generator* to create **100 more random items**. Do not add them to the inventory; instead record their names in a vector called `missing`.
2. For each name in the `contained` and in `missing`, time how long it takes to execute **only `contains()`** in **ms** and **sum the individal execution times.** Remember, the only thing between starting and ending the timer should be your contains method.
3. **Divide the summed elapsed time by 200** to get the average `contains()` time for each inventory type.
4. Record your findings in a table.

### Part B: Timing `query()`
Consider: 
```
n = { 1000, 2000, 4000, 8000 }
```

For each Inventory type listed above and each value of n, we'll execute 2 sets of tests:
#### CompareItemName test
1. Create an Inventory with `Comparator` equals `CompareItemName`
2. Create a new `ItemGenerator` with seed 42. Insert **n randomly generated items** into the Inventory.
3. Now repeat these step 10 times: 
    **a.** Get two randomly used names from the generator, and assign them to Items `start` and `end`. If needed, swap to ensure `start` < `end`.
    **b.** Time and execute `query(start, end)`. Remember, the only thing between starting and ending the timer should be your `query()` call. Add the elapsed time to a variable tracking the sum of the `query()` execution times.
4. Lastly, divide the sum by 10 to get the average `query()` time. Record your findings.

#### CompareItemWeight test
1. Create an Inventory with `Comparator` equals `CompareItemWeight`
2. Create a new `ItemGenerator` with seed 42. Insert **n randomly generated items** into the Inventory.
3. Repeat 10 times: 
    **a.** Call `randomFloat` from the generator with `MIN_WEIGHT` and `MAX_WEIGHT` as the parameters. Wrap this value in an Item object called `start`, and **create another Item `end` that is `0.1` weight_ above the start value.**
    **b.** Time and execute `query(start, end)`. Add the execution time to a sum.
4. Lastly, divide the sum by 10 to get the average `query()` time. Record your findings.

*If you feel inclined, you can time `CompareItemType` Inventories using `ItemGenerator::randomType()` but we won't ask you to. 

### Part C: Reporting Your Findings
1. Record two tables of your findings, one for **Part A** and **Part B.** 
2. Write up a brief conclusion of your findings. When I say brief, not "1-sentence brief." Give us a paragraph to work with (please)! Talk about whether you expected to see what you saw, **and why.** Also, include when and **why** you might want to use a Hash-based inventory over a Tree-based inventory and vice versa. 

No need to write too much; more work is not always better work.

## Submission Guidelines
You'll be submitting:
1. A report of your avg. execution times & findings (see above)
2. `Item.cpp`
3. `Item.hpp`
4. `Compare.cpp`
5. `Compare.hpp`
6. `HashInventory.cpp`
7. `HashInventory.hpp`
6. `ItemAVL.cpp`
7. `ItemAVL.hpp`
8. `TreeInventory.cpp`
9. `TreeInventory.hpp`
