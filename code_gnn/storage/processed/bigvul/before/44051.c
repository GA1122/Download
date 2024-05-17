first_named_child(xmlNode * parent, const char *name)
{
    xmlNode *match = NULL;

    for (match = __xml_first_child(parent); match != NULL; match = __xml_next(match)) {
         
        if (name == NULL || strcmp((const char *)match->name, name) == 0) {
            return match;
        }
    }
    return NULL;
}
