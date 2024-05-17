static int hpet_pre_load(void *opaque)
{
    HPETState *s = opaque;

     
    s->num_timers = HPET_MIN_TIMERS;
     return 0;
 }
