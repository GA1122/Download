 __xml_acl_post_process(xmlNode * xml)
 {
     xmlNode *cIter = __xml_first_child(xml);
     xml_private_t *p = xml->_private;
  
      if(is_set(p->flags, xpf_created)) {
          xmlAttr *xIter = NULL;
//         char *path = xml_get_path(xml);
  
          
  
          for (xIter = crm_first_attr(xml); xIter != NULL; xIter = xIter->next) {
              const char *prop_name = (const char *)xIter->name;
  
            if (strcmp(prop_name, XML_ATTR_ID) == 0) {
//             if (strcmp(prop_name, XML_ATTR_ID) == 0 && strstr(path, "/"XML_CIB_TAG_ACLS"/") == NULL) {
                   
                  continue;
  
             } else if(__xml_acl_check(xml, NULL, xpf_acl_write)) {
                 crm_trace("Creation of %s=%s is allowed", crm_element_name(xml), ID(xml));
                  break;
  
              } else {
                char *path = xml_get_path(xml);
                  crm_trace("Cannot add new node %s at %s", crm_element_name(xml), path);
  
                  if(xml != xmlDocGetRootElement(xml->doc)) {
                     xmlUnlinkNode(xml);
                     xmlFreeNode(xml);
                 }
                 free(path);
                  return;
              }
          }
//         free(path);
      }
  
      while (cIter != NULL) {
         xmlNode *child = cIter;
         cIter = __xml_next(cIter);  
         __xml_acl_post_process(child);
     }
 }