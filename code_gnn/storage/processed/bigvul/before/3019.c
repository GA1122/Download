uint32_t vga_mem_readb(VGACommonState *s, hwaddr addr)
{
    int memory_map_mode, plane;
    uint32_t ret;

     
    memory_map_mode = (s->gr[VGA_GFX_MISC] >> 2) & 3;
    addr &= 0x1ffff;
    switch(memory_map_mode) {
    case 0:
        break;
    case 1:
        if (addr >= 0x10000)
            return 0xff;
        addr += s->bank_offset;
        break;
    case 2:
        addr -= 0x10000;
        if (addr >= 0x8000)
            return 0xff;
        break;
    default:
    case 3:
        addr -= 0x18000;
        if (addr >= 0x8000)
            return 0xff;
        break;
    }

    if (sr(s, VGA_SEQ_MEMORY_MODE) & VGA_SR04_CHN_4M) {
         
        assert(addr < s->vram_size);
        ret = s->vram_ptr[addr];
    } else if (s->gr[VGA_GFX_MODE] & 0x10) {
         
        plane = (s->gr[VGA_GFX_PLANE_READ] & 2) | (addr & 1);
        addr = ((addr & ~1) << 1) | plane;
        if (addr >= s->vram_size) {
            return 0xff;
        }
        ret = s->vram_ptr[addr];
    } else {
         
        if (addr * sizeof(uint32_t) >= s->vram_size) {
            return 0xff;
        }
        s->latch = ((uint32_t *)s->vram_ptr)[addr];

        if (!(s->gr[VGA_GFX_MODE] & 0x08)) {
             
            plane = s->gr[VGA_GFX_PLANE_READ];
            ret = GET_PLANE(s->latch, plane);
        } else {
             
            ret = (s->latch ^ mask16[s->gr[VGA_GFX_COMPARE_VALUE]]) &
                mask16[s->gr[VGA_GFX_COMPARE_MASK]];
            ret |= ret >> 16;
            ret |= ret >> 8;
            ret = (~ret) & 0xff;
        }
    }
    return ret;
}