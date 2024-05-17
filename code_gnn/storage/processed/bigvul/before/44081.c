set_parent_flag(xmlNode *xml, long flag) 
{

    for(; xml; xml = xml->parent) {
        xml_private_t *p = xml->_private;

        if(p == NULL) {
             
        } else {
            p->flags |= flag;
             
        }
    }
}
