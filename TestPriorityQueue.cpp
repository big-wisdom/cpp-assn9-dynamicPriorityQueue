
#include "priority_queue.hpp"

#include "gtest/gtest.h"
#include <string>

// Set this to false to remove the debugging cout statements
constexpr bool DEBUG_PRINT = true;

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

template <typename T>
void reportPQ(std::string title, usu::priority_queue<T>& pq)
{
    if constexpr (DEBUG_PRINT)
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
}

TEST(MemberMethods, AddItems)
{
    usu::priority_queue<std::string> pq;
    EXPECT_EQ(pq.size(), 0);
    EXPECT_EQ(pq.empty(), true);

    pq.enqueue("a", 1);
    EXPECT_EQ(pq.size(), 1);
    EXPECT_EQ(pq.empty(), false);
    EXPECT_EQ((*pq.begin()).value, "a");
    EXPECT_EQ((*pq.begin()).priority, 1u);

    pq.enqueue("b", 2);
    EXPECT_EQ(pq.size(), 2);
    EXPECT_EQ(pq.empty(), false);
    EXPECT_EQ((*pq.begin()).value, "b");
    EXPECT_EQ((*pq.begin()).priority, 2u);

    pq.enqueue("c", 3);
    EXPECT_EQ(pq.size(), 3);
    EXPECT_EQ(pq.empty(), false);
    EXPECT_EQ((*pq.begin()).value, "c");
    EXPECT_EQ((*pq.begin()).priority, 3u);

    pq.enqueue("d", 1);
    EXPECT_EQ(pq.size(), 4);
    EXPECT_EQ(pq.empty(), false);
    EXPECT_EQ((*pq.begin()).value, "c");
    EXPECT_EQ((*pq.begin()).priority, 3u);

    pq.enqueue("e", 4);
    EXPECT_EQ(pq.size(), 5);
    EXPECT_EQ(pq.empty(), false);
    EXPECT_EQ((*pq.begin()).value, "e");
    EXPECT_EQ((*pq.begin()).priority, 4u);

    pq.enqueue("f", 2);
    EXPECT_EQ(pq.size(), 6);
    EXPECT_EQ(pq.empty(), false);
    EXPECT_EQ((*pq.begin()).value, "e");
    EXPECT_EQ((*pq.begin()).priority, 4u);

    reportPQ("--- After Add Items ---", pq);
}

TEST(MemberMethods, Misc)
{
    usu::priority_queue<std::string> pq;

    EXPECT_TRUE(pq.empty());
    EXPECT_TRUE(pq.find("a") == pq.end());

    pq.enqueue("a", 1);
    EXPECT_TRUE(pq.find("a") == pq.begin());
    EXPECT_EQ((*pq.find("a")).value, "a");
    EXPECT_EQ((*pq.find("a")).priority, 1u);

    pq.enqueue("b", 2);
    EXPECT_TRUE(pq.find("a") != pq.begin());
    EXPECT_EQ((*pq.find("a")).value, "a");
    EXPECT_EQ((*pq.find("a")).priority, 1u);

    EXPECT_TRUE(pq.find("b") == pq.begin());
    EXPECT_EQ((*pq.find("b")).value, "b");
    EXPECT_EQ((*pq.find("b")).priority, 2u);

    auto top = pq.dequeue();
    EXPECT_EQ(top.value, "b");
    EXPECT_EQ(top.priority, 2u);

    EXPECT_TRUE(pq.find("a") == pq.begin());
    EXPECT_EQ((*pq.find("a")).value, "a");
    EXPECT_EQ((*pq.find("a")).priority, 1u);

    pq.dequeue();
    EXPECT_TRUE(pq.empty());
    EXPECT_TRUE(pq.find("a") == pq.end());
    EXPECT_TRUE(pq.find("b") == pq.end());

    try
    {
        pq.dequeue();
        EXPECT_TRUE(false) << "Did not throw an exception on empty queue";
    }
    catch (...) // Using ... to prevent a polymorphic warning from g++
    {
        EXPECT_TRUE(true);
    }
}

TEST(Iterators, BeginEnd)
{
    usu::priority_queue<std::string> pq;

    EXPECT_TRUE(pq.begin() == pq.end()) << ".begin() and .end() on empty collection should be equal";

    pq.enqueue("a", 1);
    pq.enqueue("b", 2);
    pq.enqueue("c", 3);
    pq.enqueue("d", 4);
    pq.enqueue("e", 5);

    EXPECT_TRUE(pq.begin() == pq.begin()) << ".begin() and another .begin() should be equal";
    EXPECT_FALSE(pq.begin() == pq.end()) << ".begin() and .end() should not be equal on a non-empty collection (equality operator)";
    EXPECT_TRUE(pq.begin() != pq.end()) << ".begin() and .end() should not be equal on a non-empty collection (inequality operator)";
}

TEST(Iterators, Comparisons)
{
    usu::priority_queue<std::string> pq1;
    usu::priority_queue<std::string> pq2;

    pq1.enqueue("a", 1);
    pq2.enqueue("a", 1);

    EXPECT_TRUE(pq1.begin() == pq1.begin()) << ".begin() and another .begin() from same priority queue should be equal";
    EXPECT_TRUE(pq2.begin() == pq2.begin()) << ".begin() and another .begin() from same priority queue should be equal";

    EXPECT_FALSE(pq1.begin() == pq2.begin()) << ".begin() and another .begin() from different priority queue should not be equal";
    EXPECT_TRUE(pq1.begin() != pq2.begin()) << ".begin() and another .begin() from different priority queue should not be equal";
}

TEST(Iterators, Dereferenceable)
{
    usu::priority_queue<std::string> pq;

    pq.enqueue("a", 1);

    auto first = pq.begin();
    // -> operator
    EXPECT_EQ(first->value, "a");
    EXPECT_EQ(first->priority, 1u);

    // * operator
    EXPECT_EQ((*first).value, "a");
    EXPECT_EQ((*first).priority, 1u);
}


TEST(Iterators, Incrementable)
{
    usu::priority_queue<std::string> pq;

    pq.enqueue("a", 1);
    pq.enqueue("b", 2);
    pq.enqueue("c", 3);
    pq.enqueue("d", 4);
    pq.enqueue("e", 5);

    // Pre-increment tests
    auto i = pq.begin();
    ++i;
    EXPECT_FALSE(i == pq.end()) << "i should not be equal to .end()";
    EXPECT_FALSE(i == pq.begin()) << "i should not be equal to .begin()";
    ++i;
    EXPECT_FALSE(i == pq.end()) << "i should not be equal to .end()";
    ++i;
    EXPECT_FALSE(i == pq.end()) << "i should not be equal to .end()";
    ++i;
    EXPECT_FALSE(i == pq.end()) << "i should not be equal to .end()";
    ++i;
    EXPECT_TRUE(i == pq.end()) << "i should be equal to .end()";

    // Post-increment tests
    i = pq.begin();
    i++;
    EXPECT_FALSE(i == pq.end()) << "i should not be equal to .end()";
    EXPECT_FALSE(i == pq.begin()) << "i should not be equal to .begin()";
    i++;
    EXPECT_FALSE(i == pq.end()) << "i should not be equal to .end()";
    i++;
    EXPECT_FALSE(i == pq.end()) << "i should not be equal to .end()";
    i++;
    EXPECT_FALSE(i == pq.end()) << "i should not be equal to .end()";
    i++;
    EXPECT_TRUE(i == pq.end()) << "i should be equal to .end()";
}

TEST(Iterators, ConstructableAssignable)
{
    usu::priority_queue<std::string> pq;

    pq.enqueue("a", 1);
    pq.enqueue("b", 2);
    pq.enqueue("c", 3);
    pq.enqueue("d", 4);
    pq.enqueue("e", 5);

    // Copy constructor/assignment
    {
        auto i1 = pq.begin();                          // copy constructor
        usu::priority_queue<std::string>::iterator i2; // default constructor
        i2 = i1;                                       // copy assignment;
        auto i3 = i1;                                  // copy constructor

        EXPECT_EQ((*i1).value, (*i2).value) << "i1 and i2 should be equal";
        EXPECT_EQ((*i1).priority, (*i2).priority) << "i1 and i2 should be equal";

        EXPECT_EQ((*i1).value, (*i3).value) << "i1 and i3 should be equal";
        EXPECT_EQ((*i1).priority, (*i3).priority) << "i1 and i3 should be equal";

        EXPECT_EQ((*i2).value, (*i3).value) << "i2 and i3 should be equal";
        EXPECT_EQ((*i2).priority, (*i3).priority) << "i2 and i3 should be equal";

        i1++;
        EXPECT_NE((*i1).value, (*i2).value) << "i1 and i2 should not be equal";
        EXPECT_NE((*i1).priority, (*i2).priority) << "i1 and i2 should not be equal";
        EXPECT_NE((*i1).value, (*i3).value) << "i1 and i3 should not be equal";
        EXPECT_NE((*i1).priority, (*i3).priority) << "i1 and i3 should not be equal";

        i2++;
        EXPECT_EQ((*i1).value, (*i2).value) << "i1 and i2 should be equal";
        EXPECT_EQ((*i1).priority, (*i2).priority) << "i1 and i2 should be equal";
        EXPECT_NE((*i2).value, (*i3).value) << "i3 and i3 should not be equal";
        EXPECT_NE((*i2).priority, (*i3).priority) << "i3 and i3 should not be equal";
    }
    // Move constructor/assignment
    {
        auto i1 = std::move(pq.begin());               // move constructor
        usu::priority_queue<std::string>::iterator i2; // default constructor
        i2 = std::move(i1);                            // move assignment
        auto i3 = pq.begin();                          // copy constructor

        EXPECT_EQ((*i3).value, (*i2).value) << "i3 and i2 should be equal";
        EXPECT_EQ((*i3).priority, (*i2).priority) << "i3 and i2 should be equal";

        EXPECT_EQ((*i2).value, (*i3).value) << "i2 and i3 should be equal";
        EXPECT_EQ((*i2).priority, (*i3).priority) << "i2 and i3 should be equal";

        i2++;
        EXPECT_NE((*i2).value, (*i3).value) << "i2 and i3 should not be equal";
        EXPECT_NE((*i2).priority, (*i3).priority) << "i2 and i3 should not be equal";

        i3++;
        EXPECT_EQ((*i2).value, (*i3).value) << "i2 and i3 should be equal";
        EXPECT_EQ((*i2).priority, (*i3).priority) << "i2 and i3 should be equal";
    }
}

TEST(Update, UpdateItems)
{
    usu::priority_queue<std::string> pq;

    pq.enqueue("a", 1);
    pq.enqueue("b", 2);
    pq.enqueue("c", 3);
    pq.enqueue("d", 4);
    pq.enqueue("e", 5);

    reportPQ("--- Update Items: after initial add ---", pq);

    {
        auto [value, priority] = *pq.begin();

        EXPECT_EQ(value, "e");
        EXPECT_EQ(priority, 5u);
    }

    {
        pq.update(pq.find("a"), 6);
        auto [value, priority] = *pq.begin();
        reportPQ("--- Update Items: update a to 6 ---", pq);

        EXPECT_EQ(value, "a");
        EXPECT_EQ(priority, 6u);
    }

    {
        pq.update(pq.find("a"), 2);
        auto [value, priority] = *pq.begin();
        reportPQ("--- Update Items: update a to 2 ---", pq);

        EXPECT_EQ(value, "e");
        EXPECT_EQ(priority, 5u);
    }

    {
        pq.update(pq.find("e"), 1);
        auto [value, priority] = *pq.begin();
        reportPQ("--- Update Items: update e to 1 ---", pq);

        EXPECT_EQ(value, "d");
        EXPECT_EQ(priority, 4u);
    }
}
