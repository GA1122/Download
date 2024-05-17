static uint32_t timer_enabled(HPETTimer *t)
{
    return t->config & HPET_TN_ENABLE;
}
