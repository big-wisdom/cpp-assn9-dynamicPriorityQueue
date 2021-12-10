#include "priority_queue.hpp"

#include <algorithm>
#include <iostream>
#include <random>
#include <string>

template <typename T>
void reportPQ(std::string title, usu::priority_queue<T>& pq)
{
    if (!title.empty())
    {
        std::cout << title << std::endl;
    }
    for (auto [value, priority] : pq)
    {
        std::cout << value << "(" << priority << ")"
                  << " ";
    }
    std::cout << std::endl;
}

void simpleExample();
void simulation();

// ------------------------------------------------------------------
//
// Final project, a dynamically updatable priority queue.
//
// ------------------------------------------------------------------
int main()
{
    simpleExample();
    //simulation();

    return 0;
}

void simpleExample()
{
    using namespace std::string_literals;
    usu::priority_queue<std::string> pq;

    pq.enqueue("a", 1);
    pq.enqueue("b", 2);
    pq.enqueue("c", 3);
    pq.enqueue("d", 1);
    pq.enqueue("e", 4);
    pq.enqueue("f", 2);

    // Example of using the iterator with an std::algorithm, isn't that cool!
    // Find the first value "less than d"
    auto f = std::find_if(pq.begin(), pq.end(), [](auto value) { return value.value < "d"; });
    if (f != pq.end())
    {
        std::cout << "--- Found item using std::find_if(...): ";
        std::cout << f->value << "(" << f->priority << ")" << std::endl;
        std::cout << std::endl;
    }

    // Simple example of using the methods on the priority queue itself.
    auto a = pq.find("a");
    reportPQ("--- Before ---", pq);
    pq.update(a, 5);
    reportPQ("--- After Updating a ---", pq);
    pq.update(pq.find("b"), 3);
    reportPQ("--- After Updating b ---", pq);
    pq.update(pq.find("b"), 4);
    reportPQ("--- After Updating b ---", pq);
    pq.update(pq.find("b"), 5);
    reportPQ("--- After Updating b ---", pq);
    pq.update(pq.find("b"), 6);
    reportPQ("--- After Updating b ---", pq);

    std::cout << "--- Emptying The Queue ---" << std::endl;
    while (!pq.empty())
    {
        auto value = pq.dequeue();
        reportPQ("", pq);
    }

    usu::priority_queue<std::string> pq2{{"a"s, 1},
                                         {"b"s, 2},
                                         {"c"s, 3},
                                         {"d"s, 1},
                                         {"e"s, 4},
                                         {"f"s, 2}};

    reportPQ("--- Initialized from std::initializer_list ---", pq2);
}

void simulation()
{
    auto nextName = 1u; // The next "name" of the item to add to the pq
    usu::priority_queue<unsigned int, double> pq;

    // Initially generate a hundred items, with normally distributed priority
    std::random_device rd;
    std::mt19937 generator(rd());
    std::normal_distribution<float> distribution(300.0f, 50.0f);

    for (auto i = 1u; i <= 100u; i++)
    {
        auto priority = distribution(generator);
        pq.enqueue(nextName++, priority);
    }

    // Randomly choose an item in the pq boost it to the top and pull it off
    while (!pq.empty())
    {
        auto priorityMin = distribution(generator);
        auto priorityMax = distribution(generator);
        if (priorityMin > priorityMax)
        {
            std::swap(priorityMin, priorityMax);
        }
        auto boostMe = std::find_if(pq.begin(), pq.end(), [=](auto item) { return item.priority >= priorityMin && item.priority <= priorityMax; });
        if (boostMe != pq.end())
        {
            std::cout << "Remaining highest priority: " << pq.begin()->value << "(" << pq.begin()->priority << ")" << std::endl;
            std::cout << "boosted and removed..." << std::endl;
            std::cout << "\tbefore : " << boostMe->value << "(" << boostMe->priority << ")" << std::endl;
            pq.update(boostMe, (*pq.begin()).priority + 1);
            auto top = pq.dequeue();
            std::cout << "\tafter  : " << top.value << "(" << top.priority << ")" << std::endl;
        }
    }
}
