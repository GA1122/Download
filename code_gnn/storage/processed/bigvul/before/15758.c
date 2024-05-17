static uint32_t timer_fsb_route(HPETTimer *t)
{
    return t->config & HPET_TN_FSB_ENABLE;
}
