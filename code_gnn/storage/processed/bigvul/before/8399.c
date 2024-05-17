void virtqueue_get_avail_bytes(VirtQueue *vq, unsigned int *in_bytes,
                               unsigned int *out_bytes,
                               unsigned max_in_bytes, unsigned max_out_bytes)
{
    unsigned int idx;
    unsigned int total_bufs, in_total, out_total;

    idx = vq->last_avail_idx;

    total_bufs = in_total = out_total = 0;
    while (virtqueue_num_heads(vq, idx)) {
        VirtIODevice *vdev = vq->vdev;
        unsigned int max, num_bufs, indirect = 0;
        VRingDesc desc;
        hwaddr desc_pa;
        int i;

        max = vq->vring.num;
        num_bufs = total_bufs;
        i = virtqueue_get_head(vq, idx++);
        desc_pa = vq->vring.desc;
        vring_desc_read(vdev, &desc, desc_pa, i);

        if (desc.flags & VRING_DESC_F_INDIRECT) {
            if (desc.len % sizeof(VRingDesc)) {
                error_report("Invalid size for indirect buffer table");
                exit(1);
            }

             
            if (num_bufs >= max) {
                error_report("Looped descriptor");
                exit(1);
            }

             
            indirect = 1;
            max = desc.len / sizeof(VRingDesc);
            desc_pa = desc.addr;
            num_bufs = i = 0;
            vring_desc_read(vdev, &desc, desc_pa, i);
        }

        do {
             
            if (++num_bufs > max) {
                error_report("Looped descriptor");
                exit(1);
            }

            if (desc.flags & VRING_DESC_F_WRITE) {
                in_total += desc.len;
            } else {
                out_total += desc.len;
            }
            if (in_total >= max_in_bytes && out_total >= max_out_bytes) {
                goto done;
            }
        } while ((i = virtqueue_read_next_desc(vdev, &desc, desc_pa, max)) != max);

        if (!indirect)
            total_bufs = num_bufs;
        else
            total_bufs++;
    }
done:
    if (in_bytes) {
        *in_bytes = in_total;
    }
    if (out_bytes) {
        *out_bytes = out_total;
    }
}
