ofputil_decode_hello_bitmap(const struct ofp_hello_elem_header *oheh,
                            uint32_t *allowed_versionsp)
{
    uint16_t bitmap_len = ntohs(oheh->length) - sizeof *oheh;
    const ovs_be32 *bitmap = ALIGNED_CAST(const ovs_be32 *, oheh + 1);
    uint32_t allowed_versions;

    if (!bitmap_len || bitmap_len % sizeof *bitmap) {
        return false;
    }

     
    allowed_versions = ntohl(bitmap[0]);

    if (allowed_versions & 1) {
         
        VLOG_WARN_RL(&bad_ofmsg_rl, "peer claims to support invalid OpenFlow "
                     "version 0x00");
        allowed_versions &= ~1u;
    }

    if (!allowed_versions) {
        VLOG_WARN_RL(&bad_ofmsg_rl, "peer does not support any OpenFlow "
                     "version (between 0x01 and 0x1f)");
        return false;
    }

    *allowed_versionsp = allowed_versions;
    return true;
}
