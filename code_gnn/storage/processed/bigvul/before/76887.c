encode_SET_FIELD(const struct ofpact_set_field *sf,
                 enum ofp_version ofp_version, struct ofpbuf *out)
{
    if (ofp_version >= OFP15_VERSION) {
         
        set_field_to_set_field(sf, ofp_version, out);
    } else if (sf->ofpact.raw == NXAST_RAW_REG_LOAD ||
               sf->ofpact.raw == NXAST_RAW_REG_LOAD2) {
         
        set_field_to_nxast(sf, out);
    } else if (ofp_version < OFP12_VERSION) {
         
        set_field_to_legacy_openflow(sf, ofp_version, out);
    } else if (is_all_ones(ofpact_set_field_mask(sf), sf->field->n_bytes)) {
         
        set_field_to_set_field(sf, ofp_version, out);
    } else {
         
        set_field_to_nxast(sf, out);
    }
}
