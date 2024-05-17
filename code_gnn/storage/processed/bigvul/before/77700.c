ofputil_put_switch_features_port(const struct ofputil_phy_port *pp,
                                 struct ofpbuf *b)
{
    const struct ofp_header *oh = b->data;

    if (oh->version < OFP13_VERSION) {
         
        size_t start_ofs = b->size;
        ofputil_put_phy_port(oh->version, pp, b);
        if (b->size > UINT16_MAX) {
            b->size = start_ofs;
        }
    }
}
