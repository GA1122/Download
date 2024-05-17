    OVS_REQUIRES(ofproto_mutex)
{
    if ((c->table_id == rule->table_id || c->table_id == 0xff)
        && ofproto_rule_has_out_port(rule, c->out_port)
        && ofproto_rule_has_out_group(rule, c->out_group)
        && !((rule->flow_cookie ^ c->cookie) & c->cookie_mask)
        && (!rule_is_hidden(rule) || c->include_hidden)
        && cls_rule_visible_in_version(&rule->cr, c->version)) {
         
        if (!rule_is_readonly(rule) || c->include_readonly) {
             
            rule_collection_add(rules, rule);
        } else {
             
            ++*n_readonly;
        }
    }
}
