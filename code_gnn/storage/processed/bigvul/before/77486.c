ofputil_capabilities_mask(enum ofp_version ofp_version)
{
     
    switch (ofp_version) {
    case OFP10_VERSION:
    case OFP11_VERSION:
        return OFPC_COMMON | OFPC_ARP_MATCH_IP;
    case OFP12_VERSION:
    case OFP13_VERSION:
        return OFPC_COMMON | OFPC12_PORT_BLOCKED;
    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION:
        return OFPC_COMMON | OFPC12_PORT_BLOCKED | OFPC14_BUNDLES
            | OFPC14_FLOW_MONITORING;
    default:
         
        return 0;
    }
}
