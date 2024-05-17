static unsigned virtqueue_next_desc(hwaddr desc_pa,
                                    unsigned int i, unsigned int max)
{
    unsigned int next;

     
    if (!(vring_desc_flags(desc_pa, i) & VRING_DESC_F_NEXT))
        return max;

     
    next = vring_desc_next(desc_pa, i);
     
    smp_wmb();

    if (next >= max) {
        error_report("Desc next is %u", next);
        exit(1);
    }

    return next;
}
