ofputil_async_cfg_default(enum ofp_version version)
{
     
    uint32_t pin = OFPR14_BITS & ~(1u << OFPR_INVALID_TTL);
    pin |= 1u << OFPR_EXPLICIT_MISS;
    if (version <= OFP12_VERSION) {
        pin |= 1u << OFPR_IMPLICIT_MISS;
    }

    return (struct ofputil_async_cfg) {
        .master[OAM_PACKET_IN] = pin,

        .master[OAM_FLOW_REMOVED]
            = (version >= OFP14_VERSION ? OFPRR14_BITS : OFPRR10_BITS),

        .master[OAM_PORT_STATUS] = OFPPR_BITS,
        .slave[OAM_PORT_STATUS] = OFPPR_BITS,
    };
}
