    OVS_REQUIRES(ofproto_mutex)
{
     
    group->being_deleted = true;

     
    delete_flows_start__(ofproto, version, &group->rules);
    group_collection_add(groups, group);
    versions_set_remove_version(&group->versions, version);
    ofproto->n_groups[group->type]--;
}
