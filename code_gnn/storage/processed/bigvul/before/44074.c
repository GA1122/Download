pcmkRegisterNode(xmlNodePtr node)
{
    xml_private_t *p = NULL;

    switch(node->type) {
        case XML_ELEMENT_NODE:
        case XML_DOCUMENT_NODE:
        case XML_ATTRIBUTE_NODE:
        case XML_COMMENT_NODE:
            p = calloc(1, sizeof(xml_private_t));
            p->check = XML_PRIVATE_MAGIC;
             
            p->flags |= (xpf_dirty|xpf_created);
            node->_private = p;
            break;
        case XML_TEXT_NODE:
        case XML_DTD_NODE:
            break;
        default:
             
            crm_trace("Ignoring %p %d", node, node->type);
            CRM_LOG_ASSERT(node->type == XML_ELEMENT_NODE);
            break;
    }

    if(p && TRACKING_CHANGES(node)) {
         
        set_doc_flag(node, xpf_dirty);
        __xml_node_dirty(node);
    }
}
