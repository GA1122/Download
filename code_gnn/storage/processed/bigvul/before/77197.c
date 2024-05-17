    OVS_REQUIRES(ofproto_mutex)
{
    struct ofgroup *new_group = ogm->new_group;
    struct ofgroup *old_group;

     
    GROUP_COLLECTION_FOR_EACH (old_group, &ogm->old_groups) {
        ofproto->n_groups[old_group->type]++;
        if (new_group) {
            ovs_assert(group_collection_n(&ogm->old_groups) == 1);
             
            rule_collection_move(&old_group->rules, &new_group->rules);
        } else {
            old_group->being_deleted = false;
             
            delete_flows_revert__(ofproto, &old_group->rules);
        }
         
        versions_set_remove_version(&old_group->versions,
                                    OVS_VERSION_NOT_REMOVED);
    }
    if (new_group) {
         
        cmap_remove(&ofproto->groups, &new_group->cmap_node,
                    hash_int(new_group->group_id, 0));
        ofproto->n_groups[new_group->type]--;
        ofproto_group_unref(new_group);
    }
}
