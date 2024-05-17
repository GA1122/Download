static uint32_t hpet_in_legacy_mode(HPETState *s)
{
    return s->config & HPET_CFG_LEGACY;
}
