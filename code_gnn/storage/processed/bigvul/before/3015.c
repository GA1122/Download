int vga_ioport_invalid(VGACommonState *s, uint32_t addr)
{
    if (s->msr & VGA_MIS_COLOR) {
         
        return (addr >= 0x3b0 && addr <= 0x3bf);
    } else {
         
        return (addr >= 0x3d0 && addr <= 0x3df);
    }
}
