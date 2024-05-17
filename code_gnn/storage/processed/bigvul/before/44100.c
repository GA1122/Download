xml_acl_disable(xmlNode *xml)
{
    if(xml_acl_enabled(xml)) {
        xml_private_t *p = xml->doc->_private;

         
        __xml_acl_apply(xml);
        __xml_acl_post_process(xml);
        clear_bit(p->flags, xpf_acl_enabled);
    }
}