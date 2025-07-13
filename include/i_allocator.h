// type erased allocator

class IAllocator
{
public:
    IAllocator(/* args */);
    ~IAllocator();
};

template <typename Strategy>
class AllocatorWrapper : IAllocator {
public:
    AllocatorWrapper();
private:
    Strategy strategy_;
};
