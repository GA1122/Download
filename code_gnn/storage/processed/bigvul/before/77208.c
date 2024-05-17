    OVS_REQUIRES(ofproto_mutex)
{
     
    long long int expiration = LLONG_MAX;
    if (rule->hard_timeout) {
         
        ovs_mutex_lock(&rule->mutex);
        long long int modified = rule->modified;
        ovs_mutex_unlock(&rule->mutex);

        expiration = modified + rule->hard_timeout * 1000;
    }
    if (rule->idle_timeout) {
        uint64_t packets, bytes;
        long long int used;
        long long int idle_expiration;

        ofproto->ofproto_class->rule_get_stats(rule, &packets, &bytes, &used);
        idle_expiration = used + rule->idle_timeout * 1000;
        expiration = MIN(expiration, idle_expiration);
    }
    if (expiration == LLONG_MAX) {
        return 0;
    }

     
    uint32_t expiration_ofs = (expiration >> 10) - (time_boot_msec() >> 10);

     
    uint64_t priority = ((uint64_t) rule->importance << 32) + expiration_ofs;
    return UINT64_MAX - priority;
}
