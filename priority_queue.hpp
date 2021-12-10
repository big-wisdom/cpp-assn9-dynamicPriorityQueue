#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <utility>

namespace usu 
{

    template <typename R, typename priority_type = unsigned int>
    class priority_queue
    {
        public:
            struct priority_item
            {
                priority_item(R v, priority_type r) : 
                    value(v),
                    priority_type(r)
                {
                };
                priority_item() :
                    priority_item(0, 0) 
                {
                };

                priority_item(std::pair<R, priority_type> pair) :
                    value(pair.first), priority(pair.second)
                {
                };

                R value;
                priority_type priority;
            };

            class iterator : public std::iterator<std::forward_iterator_tag, priority_queue*>
            {
                public:

                    using iterator_category = std::forward_iterator_tag;

                    iterator() : 
                        iterator(nullptr)
                    {
                    };

                    iterator(std::vector<priority_item>* ptr) :
                        m_data(ptr),
                        m_pos(0)
                    {
                    };

                    iterator(size_t pos, std::vector<priority_item>* ptr) :
                        m_pos(pos),
                        m_data(ptr)
                    {
                    };

                    iterator(const iterator& obj);
                    iterator(iterator&& obj) noexcept;
                    iterator& operator=(const iterator& rhs); 
                    iterator& operator=(iterator&& rhs);
                    iterator operator++();    // incrementable e.g., ++r
                    iterator operator++(int);
                    bool operator==(const iterator& rhs) { return m_pos == rhs.m_pos && m_data == rhs.m_data; }
                    bool operator!=(const iterator& rhs) { return m_pos != rhs.m_pos && m_data != m_data; }
                    priority_item* operator->()
                    {
                        priority_item p = (*m_data)[m_pos];
                        priority_item* ptr = new priority_item(p.value, p.priority);
                        return ptr;
                    }
                    priority_item& operator*()
                    {
                        return (*m_data)[m_pos];
                    }

                private:
                    size_t m_pos;
                    std::vector<priority_item>* m_data;
            };

            priority_queue() : queue({}) {};
            priority_queue(const std::initializer_list<std::pair<R, priority_type>>& list);

            std::vector<priority_item> queue;

            iterator begin()
            {
                return iterator(&queue);
            }
            iterator end() 
            { 
                return iterator(queue.size(), &queue);
            }

            size_t size()
            {
                return queue.size();
            }

            bool empty()
            {
                return queue.size() == 0;
            }

            void enqueue(R value, priority_type priority)
            {
                auto curr = static_cast<int>(queue.size());
                curr = static_cast<int>(curr);
                // Start at end of heap
                // queue.insert(queue.end(), std::make_pair(value, priority));
                // mypair* m = new mypair(value, priority);
                queue.insert(queue.end(), priority_type(value, priority));

                // Now sift up until curr's parent's key > curr's key
                while ((curr != 0) && (compareTo(static_cast<int>(curr), parent(curr))))
                {
                    swapItems(static_cast<int>(curr), parent(curr));
                    curr = static_cast<int>(parent(curr));
                }
            }

            priority_type dequeue()
            {
                if (queue.size() == 0)
                { // removing from empty heap
                    throw "heap is empty!";
                }
                // std::cout << "Deqeueing" << std::endl;
                // Swap maximum with last value
                swapItems(0, static_cast<int>(queue.size() - 1));
                // std::cout << "after swap " << std::endl;
                // debug();
                auto returnPair = queue[queue.size() - 1];
                queue.resize(queue.size() - 1); // truncate array
                siftdown(0);                        // put new root in correct place
                // std::cout << "returning max value pair: ( " << returnPair.value << " , " << returnPair.priority << " ) " << std::endl;
                return returnPair;
            }

            iterator find(R value)
            {
                for (auto i = begin(); i != end(); ++i)
                {
                    if(i->value == value)
                        return i;
                }
                return end();
            }

            void update(iterator i, priority_type priority)
            {
                queue[i.getPosition()].priority = priority;
                updatePriority(i.getPosition());
            }

        private:
            void updatePriority(int pos)
            {
                while((pos > 0) && (compareTo(pos, parent(pos))))
                {
                    swapItems(pos, parent(pos));
                    pos = parent(pos);
                }

                siftdown(pos);
            }

            int parent(int pos)
            {
                if (pos <= 0)
                {
                    return -1; 
                }
                return (pos - 1) / 2;
            }

            void swapItems(int p1, int p2)
            {
                auto temp = queue[p1];
                queue[p1] = queue[p2];
                queue[p2] = temp;
            }

            void siftdown(int pos)
            {
                if((pos < 0) || (static_cast<size_t>(pos) <= queue.size()))
                {
                    return;
                }

                while (!isLeaf(pos))
                {
                    int j = leftchild(pos);
                    if((static_cast<size_t>(j) < (queue.size() - 1)) && (!compareTo(j, j+1)))
                    {
                        j++;
                    }
                    if (compareTo(pos, j))
                    {
                        return;
                    }
                    swapItems(pos, j);
                    pos = j;
                }
            }

            bool isLeaf(int pos)
            {
                return (static_cast<size_t>(pos) >= queue.size() / 2) && (static_cast<size_t>(pos) < queue.size());
            }

            int leftchild(int pos)
            {
                if(static_cast<size_t>(pos) >= queue.size() / 2)
                {
                    return -1;
                }
                return 2 * pos + 1;
            }

            int rightchild(int pos)
            {
                if (static_cast<size_t>(pos) >= (queue.size() - 1) / 2)
                {
                    return -1;
                }
                return 2 * pos + 2;
            }

            bool compareTo(int p1, int p2)
            {
                return queue[p1].priority > queue[p2].priority;
            }
    };

    template <typename R, typename priority_type>
    priority_queue<R, priority_type>::priority_queue(const std::initializer_list<std::pair<R, priority_type>>& list)
    {

        // if (list.size() > size())
        // {
        //     throw "Initializer list contains too many elements";
        // }
        // unsigned int pos = 0;
        for (auto i = list.begin(); i != list.end(); i++)
        {
            // enqueue(i*);
            this->enqueue(i->first, i->second);
        }
    }
    template <typename T, typename P>
    priority_queue<T, P>::iterator::iterator(const iterator& obj)
    {
        m_data = obj.m_data;
        m_pos = obj.m_pos;
    }
    template <typename T, typename P>
    priority_queue<T, P>::iterator::iterator(iterator&& obj) noexcept
    {
        m_data = obj.m_data;
        m_pos = obj.m_pos;
        obj.m_data = nullptr;
        obj.m_pos = 0;
    }
    template <typename T, typename P>
    typename priority_queue<T, P>::iterator& priority_queue<T, P>::iterator::operator=(const iterator& rhs)
    {
        m_data = rhs.m_data;
        m_pos = rhs.m_pos;
        return *this;
    }
    template <typename T, typename P>
    typename priority_queue<T, P>::iterator& priority_queue<T, P>::iterator::operator=(iterator&& rhs)
    {
        // if (this != rhs)
        // {
        //     std::swap(this->m_data, rhs.m_data);
        //     std::swap(this->m_pos, rhs.m_pos);
        // }
        std::swap(m_data, rhs.m_data);
        std::swap(m_pos, rhs.m_pos);
        return *this;
    }
    template <typename T, typename P>
    typename priority_queue<T, P>::iterator priority_queue<T, P>::iterator::operator++()
    {
        m_pos++;

        // m_pos = rightchild(m_pos);
        return *this;
    }
    template <typename T, typename P>
    typename priority_queue<T, P>::iterator priority_queue<T, P>::iterator::operator++(int)
    {
        iterator i = *this;
        m_pos++;
        return i;
    }
}
