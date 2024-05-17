static void hpet_pre_save(void *opaque)
{
    HPETState *s = opaque;

     
    s->hpet_counter = hpet_get_ticks(s);
}
