    OVS_EXCLUDED(ofproto_mutex)
{
    struct ofport *ofport, *next_ofport;
    struct ofport_usage *usage;

    if (!p) {
        return;
    }

    if (p->meters) {
        meter_delete(p, 1, p->meter_features.max_meters);
        p->meter_features.max_meters = 0;
        free(p->meters);
        p->meters = NULL;
    }

    ofproto_flush__(p);
    HMAP_FOR_EACH_SAFE (ofport, next_ofport, hmap_node, &p->ports) {
        ofport_destroy(ofport, del);
    }

    HMAP_FOR_EACH_POP (usage, hmap_node, &p->ofport_usage) {
        free(usage);
    }

    p->ofproto_class->destruct(p, del);

     
    ovs_mutex_lock(&ofproto_mutex);
    connmgr_destroy(p->connmgr);
    p->connmgr = NULL;
    ovs_mutex_unlock(&ofproto_mutex);

     
    ovsrcu_postpone(ofproto_destroy_defer__, p);
}
