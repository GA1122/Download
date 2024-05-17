barrier_is_vertical(const struct PointerBarrier *barrier)
{
    return barrier->x1 == barrier->x2;
}
