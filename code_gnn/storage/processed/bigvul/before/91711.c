const char *am_get_header_attr(request_rec *r, const char *h,
                               const char *v, const char *a) 
{
    const char *value;
    const char *attr;
    char *l1;
    const char *attr_value = NULL;

     
    if ((value = am_xstrtok(r, h, ";", &l1)) == NULL)
        return NULL;
    am_strip_blank(&value);

      
    if ((v != NULL) && (strcasecmp(value, v) != 0))
        return NULL;

     
    if (a == NULL)
        return h;

    while ((attr = am_xstrtok(r, NULL, ";", &l1)) != NULL) {
        const char *attr_name = NULL;
        char *l2;

        am_strip_blank(&attr);

        attr_name = am_xstrtok(r, attr, "=", &l2); 
        if ((attr_name != NULL) && (strcasecmp(attr_name, a) == 0)) {
            if ((attr_value = am_xstrtok(r, NULL, "=", &l2)) != NULL)
                am_strip_blank(&attr_value);
            break;
        }
    }
  
     
    if (attr_value != NULL) {
        apr_size_t len; 

        len = strlen(attr_value);
        if ((len > 1) && (attr_value[len - 1] == '\"'))
            attr_value = apr_pstrndup(r->pool, attr_value, len - 1);
        if (attr_value[0] == '\"')
            attr_value++;
    }
    
    return attr_value;
}
