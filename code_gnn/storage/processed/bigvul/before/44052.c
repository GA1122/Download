fix_plus_plus_recursive(xmlNode * target)
{
     
    xmlNode *child = NULL;
    xmlAttrPtr pIter = NULL;

    for (pIter = crm_first_attr(target); pIter != NULL; pIter = pIter->next) {
        const char *p_name = (const char *)pIter->name;
        const char *p_value = crm_attr_value(pIter);

        expand_plus_plus(target, p_name, p_value);
    }
    for (child = __xml_first_child(target); child != NULL; child = __xml_next(child)) {
        fix_plus_plus_recursive(child);
    }
}
