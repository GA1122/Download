table_mod(struct ofproto *ofproto, const struct ofputil_table_mod *tm)
{
    if (!check_table_id(ofproto, tm->table_id)) {
        return OFPERR_OFPTMFC_BAD_TABLE;
    }

     
    if (tm->eviction_flags != UINT32_MAX
        && tm->eviction_flags != OFPROTO_EVICTION_FLAGS) {
        return OFPERR_OFPTMFC_BAD_CONFIG;
    }

    if (tm->table_id == OFPTT_ALL) {
        struct oftable *oftable;
        OFPROTO_FOR_EACH_TABLE (oftable, ofproto) {
            if (!(oftable->flags & (OFTABLE_HIDDEN | OFTABLE_READONLY))) {
                table_mod__(oftable, tm);
            }
        }
    } else {
        struct oftable *oftable = &ofproto->tables[tm->table_id];
        if (oftable->flags & OFTABLE_READONLY) {
            return OFPERR_OFPTMFC_EPERM;
        }
        table_mod__(oftable, tm);
    }

    return 0;
}
