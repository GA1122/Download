crm_attr_dirty(xmlAttr *a) 
{
    xmlNode *parent = a->parent;
    xml_private_t *p = NULL;

    p = a->_private;
    p->flags |= (xpf_dirty|xpf_modified);
    p->flags = (p->flags & ~xpf_deleted);
     
     

    __xml_node_dirty(parent);
}
