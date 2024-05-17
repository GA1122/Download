    OVS_REQUIRES(ofproto_mutex)
{
    if (m->flags & NXFMF_INITIAL) {
        ofproto_collect_ofmonitor_refresh_rules(m, 0, rules);
    }
}
