    OVS_REQUIRES(ofproto_mutex)
{
    ofproto_collect_ofmonitor_refresh_rules(m, seqno, rules);
}
