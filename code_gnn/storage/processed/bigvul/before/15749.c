static uint64_t hpet_get_ticks(HPETState *s)
{
    return ns_to_ticks(qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) + s->hpet_offset);
}
