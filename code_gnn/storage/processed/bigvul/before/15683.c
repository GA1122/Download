static void openpic_gcr_write(OpenPICState *opp, uint64_t val)
{
    bool mpic_proxy = false;

    if (val & GCR_RESET) {
        openpic_reset(DEVICE(opp));
        return;
    }

    opp->gcr &= ~opp->mpic_mode_mask;
    opp->gcr |= val & opp->mpic_mode_mask;

     
    if ((val & opp->mpic_mode_mask) == GCR_MODE_PROXY) {
        mpic_proxy = true;
    }

    ppce500_set_mpic_proxy(mpic_proxy);
}
