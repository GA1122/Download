static bool fdc_reset_sensei_needed(void *opaque)
{
    FDCtrl *s = opaque;

    return s->reset_sensei != 0;
}
