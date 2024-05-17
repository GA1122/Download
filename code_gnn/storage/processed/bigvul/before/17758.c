barrier_is_blocking_direction(const struct PointerBarrier * barrier,
                              int direction)
{
     
    return (barrier->directions & direction) != direction;
}
