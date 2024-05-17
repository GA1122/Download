expand_plus_plus(xmlNode * target, const char *name, const char *value)
{
    int offset = 1;
    int name_len = 0;
    int int_value = 0;
    int value_len = 0;

    const char *old_value = NULL;

    if (value == NULL || name == NULL) {
        return;
    }

    old_value = crm_element_value(target, name);

    if (old_value == NULL) {
         
        goto set_unexpanded;

    } else if (strstr(value, name) != value) {
        goto set_unexpanded;
    }

    name_len = strlen(name);
    value_len = strlen(value);
    if (value_len < (name_len + 2)
        || value[name_len] != '+' || (value[name_len + 1] != '+' && value[name_len + 1] != '=')) {
        goto set_unexpanded;
    }

     
    if (old_value != value) {
        int_value = char2score(old_value);
    }

    if (value[name_len + 1] != '+') {
        const char *offset_s = value + (name_len + 2);

        offset = char2score(offset_s);
    }
    int_value += offset;

    if (int_value > INFINITY) {
        int_value = (int)INFINITY;
    }

    crm_xml_add_int(target, name, int_value);
    return;

  set_unexpanded:
    if (old_value == value) {
         
        return;
    }
    crm_xml_add(target, name, value);
    return;
}
