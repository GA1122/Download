rule_criteria_init(struct rule_criteria *criteria, uint8_t table_id,
                   const struct match *match, int priority,
                   ovs_version_t version, ovs_be64 cookie,
                   ovs_be64 cookie_mask, ofp_port_t out_port,
                   uint32_t out_group)
{
    criteria->table_id = table_id;
    cls_rule_init(&criteria->cr, match, priority);
    criteria->version = version;
    criteria->cookie = cookie;
    criteria->cookie_mask = cookie_mask;
    criteria->out_port = out_port;
    criteria->out_group = out_group;

     
    criteria->include_hidden = priority > UINT16_MAX;

     
    criteria->include_readonly = true;
}
