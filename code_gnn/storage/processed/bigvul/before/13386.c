static void i6300esb_disable_timer(I6300State *d)
{
    i6300esb_debug("timer disabled\n");

    timer_del(d->timer);
}
