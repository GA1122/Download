ofputil_parse_key_value(char **stringp, char **keyp, char **valuep)
{
     
    *stringp += strspn(*stringp, ", \t\r\n");
    if (**stringp == '\0') {
        *keyp = *valuep = NULL;
        return false;
    }

     
    char *key = *stringp;
    size_t key_len = strcspn(key, ":=(, \t\r\n");
    char key_delim = key[key_len];
    key[key_len] = '\0';
    *stringp += key_len + (key_delim != '\0');

     
    const char *value_delims;
    if (key_delim == ':' || key_delim == '=') {
        value_delims = ", \t\r\n";
    } else if (key_delim == '(') {
        value_delims = ")";
    } else {
        *keyp = key;
        *valuep = key + key_len;  
        return true;
    }

     
    char *value = *stringp;
    size_t value_len = parse_value(value, value_delims);
    char value_delim = value[value_len];
    value[value_len] = '\0';
    *stringp += value_len + (value_delim != '\0');

    *keyp = key;
    *valuep = value;
    return true;
}
