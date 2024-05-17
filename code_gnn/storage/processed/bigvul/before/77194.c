    OVS_REQUIRES(ofproto_mutex)
{
     
    delete_flows_finish__(ofproto, &group->rules, OFPRR_GROUP_DELETE, NULL);

     
}
