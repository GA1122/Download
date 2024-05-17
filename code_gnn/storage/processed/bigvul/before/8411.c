pvscsi_get_next_sg_elem(PVSCSISGState *sg)
{
    struct PVSCSISGElement elem;

    cpu_physical_memory_read(sg->elemAddr, (void *)&elem, sizeof(elem));
    if ((elem.flags & ~PVSCSI_KNOWN_FLAGS) != 0) {
         
        trace_pvscsi_get_next_sg_elem(elem.flags);
    }

    sg->elemAddr += sizeof(elem);
    sg->dataAddr = elem.addr;
    sg->resid = elem.length;
}
