ofputil_encode_table_config(enum ofputil_table_miss miss,
                            enum ofputil_table_eviction eviction,
                            enum ofputil_table_vacancy vacancy,
                            enum ofp_version version)
{
    uint32_t config = 0;
     
    switch (version) {
    case OFP10_VERSION:
         
        return htonl(0);

    case OFP11_VERSION:
    case OFP12_VERSION:
         
        switch (miss) {
        case OFPUTIL_TABLE_MISS_DEFAULT:
             
        case OFPUTIL_TABLE_MISS_CONTROLLER:
        default:
            return htonl(OFPTC11_TABLE_MISS_CONTROLLER);
        case OFPUTIL_TABLE_MISS_CONTINUE:
            return htonl(OFPTC11_TABLE_MISS_CONTINUE);
        case OFPUTIL_TABLE_MISS_DROP:
            return htonl(OFPTC11_TABLE_MISS_DROP);
        }
        OVS_NOT_REACHED();

    case OFP13_VERSION:
         
        return htonl(0);

    case OFP14_VERSION:
    case OFP15_VERSION:
    case OFP16_VERSION:
         
        if (eviction == OFPUTIL_TABLE_EVICTION_ON) {
            config |= OFPTC14_EVICTION;
        }
        if (vacancy == OFPUTIL_TABLE_VACANCY_ON) {
            config |= OFPTC14_VACANCY_EVENTS;
        }
        return htonl(config);
    }

    OVS_NOT_REACHED();
}
