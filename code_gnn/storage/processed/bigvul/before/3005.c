static bool vga_endian_state_needed(void *opaque)
{
    VGACommonState *s = opaque;

     
    return s->default_endian_fb != s->big_endian_fb;
}
