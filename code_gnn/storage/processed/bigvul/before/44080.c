set_doc_flag(xmlNode *xml, long flag) 
{

    if(xml && xml->doc && xml->doc->_private){
         
        xml_private_t *p = xml->doc->_private;

        p->flags |= flag;
         
    }
}
