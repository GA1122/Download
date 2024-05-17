static bool fdc_result_timer_needed(void *opaque)
{
    FDCtrl *s = opaque;

    return timer_pending(s->result_timer);
}
