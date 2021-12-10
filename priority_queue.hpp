#include <iostream>
#include <vector>

namespace usu 
{
    template <typename R, typename priority_type = unsigned int>
    class priority_item
    {
        R value;
        priority_type priority;
    };

    template <typename R, typename priority_type = unsigned int>
    class priority_queue
    {
        public:
            std::vector<priority_item<R, priority_type>> queue;

            void enqueue(R value, priority_type priority)
            {
                queue.push_back(priority_item(value, priority));
            }
    };
}
