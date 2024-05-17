static uint32_t timer_int_route(struct HPETTimer *timer)
{
    return (timer->config & HPET_TN_INT_ROUTE_MASK) >> HPET_TN_INT_ROUTE_SHIFT;
}
