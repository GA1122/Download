static int deactivating_bit(uint64_t old, uint64_t new, uint64_t mask)
{
    return ((old & mask) && !(new & mask));
}
