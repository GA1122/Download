static void work_around_broken_dhclient(struct virtio_net_hdr *hdr,
                                        uint8_t *buf, size_t size)
{
    if ((hdr->flags & VIRTIO_NET_HDR_F_NEEDS_CSUM) &&  
        (size > 27 && size < 1500) &&  
        (buf[12] == 0x08 && buf[13] == 0x00) &&  
        (buf[23] == 17) &&  
        (buf[34] == 0 && buf[35] == 67)) {  
        net_checksum_calculate(buf, size);
        hdr->flags &= ~VIRTIO_NET_HDR_F_NEEDS_CSUM;
    }
}
