    OVS_REQUIRES(ofproto_mutex)
{
    struct rule_collection *old_rules = &ofm->old_rules;
    struct rule_collection *new_rules = &ofm->new_rules;
    enum ofperr error;

    if (rule_collection_n(old_rules) > 0) {
         
        struct rule *old_rule, *new_rule;
        const struct rule_actions *actions = rule_get_actions(ofm->temp_rule);

         
        error = ofproto_check_ofpacts(ofproto, actions->ofpacts,
                                      actions->ofpacts_len);
        if (error) {
            return error;
        }

         
        struct rule *temp = ofm->temp_rule;
        ofm->temp_rule = NULL;    

        bool first = true;
        RULE_COLLECTION_FOR_EACH (old_rule, old_rules) {
            if (first) {
                 
                cls_rule_destroy(CONST_CAST(struct cls_rule *, &temp->cr));
                cls_rule_clone(CONST_CAST(struct cls_rule *, &temp->cr),
                               &old_rule->cr);
                if (temp->match_tlv_bitmap != old_rule->match_tlv_bitmap) {
                    mf_vl_mff_unref(&temp->ofproto->vl_mff_map,
                                    temp->match_tlv_bitmap);
                    temp->match_tlv_bitmap = old_rule->match_tlv_bitmap;
                    mf_vl_mff_ref(&temp->ofproto->vl_mff_map,
                                  temp->match_tlv_bitmap);
                }
                *CONST_CAST(uint8_t *, &temp->table_id) = old_rule->table_id;
                rule_collection_add(new_rules, temp);
                first = false;
            } else {
                struct cls_rule cr;
                cls_rule_clone(&cr, &old_rule->cr);
                error = ofproto_rule_create(ofproto, &cr, old_rule->table_id,
                                            temp->flow_cookie,
                                            temp->idle_timeout,
                                            temp->hard_timeout, temp->flags,
                                            temp->importance,
                                            temp->actions->ofpacts,
                                            temp->actions->ofpacts_len,
                                            old_rule->match_tlv_bitmap,
                                            temp->ofpacts_tlv_bitmap,
                                            &new_rule);
                if (!error) {
                    rule_collection_add(new_rules, new_rule);
                } else {
                     
                    ofm->temp_rule = temp;
                    rule_collection_rules(new_rules)[0] = NULL;

                    rule_collection_unref(new_rules);
                    rule_collection_destroy(new_rules);
                    return error;
                }
            }
        }
        ovs_assert(rule_collection_n(new_rules)
                   == rule_collection_n(old_rules));

        RULE_COLLECTIONS_FOR_EACH (old_rule, new_rule, old_rules, new_rules) {
            replace_rule_start(ofproto, ofm, old_rule, new_rule);
        }
    } else if (ofm->modify_may_add_flow) {
         
        error = add_flow_start(ofproto, ofm);
    } else {
         
        ofproto_rule_unref(ofm->temp_rule);
        ofm->temp_rule = NULL;    
        error = 0;
    }

    return error;
}
