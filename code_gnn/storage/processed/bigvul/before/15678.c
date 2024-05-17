static uint32_t openpic_cpu_read_internal(void *opaque, hwaddr addr,
                                          int idx)
{
    OpenPICState *opp = opaque;
    IRQDest *dst;
    uint32_t retval;

    DPRINTF("%s: cpu %d addr %#" HWADDR_PRIx "\n", __func__, idx, addr);
    retval = 0xFFFFFFFF;

    if (idx < 0) {
        return retval;
    }

    if (addr & 0xF) {
        return retval;
    }
    dst = &opp->dst[idx];
    addr &= 0xFF0;
    switch (addr) {
    case 0x80:  
        retval = dst->ctpr;
        break;
    case 0x90:  
        retval = idx;
        break;
    case 0xA0:  
        retval = openpic_iack(opp, dst, idx);
        break;
    case 0xB0:  
        retval = 0;
        break;
    default:
        break;
    }
    DPRINTF("%s: => 0x%08x\n", __func__, retval);

    return retval;
}
