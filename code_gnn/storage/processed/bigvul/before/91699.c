char *am_extract_query_parameter(apr_pool_t *pool,
                                 const char *query_string,
                                 const char *name)
{
    const char *ip;
    const char *value_end;
    apr_size_t namelen;

    if (query_string == NULL) {
        return NULL;
    }

    ip = query_string;
    namelen = strlen(name);

     
    for (;;) {
         
        ip = strstr(ip, name);
        if (ip == NULL) {
             
            return NULL;
        }

         
        if (ip != query_string && ip[-1] != '&') {
             
            ip++;
            continue;
        }

         
        if (ip[namelen] != '=' && ip[namelen] != '&'
            && ip[namelen] != '\0') {
             
            ip++;
            continue;
        }


         
        ip += namelen;
        break;
    }

     
    if (ip[0] == '=') {
        ip += 1;
    }

     
    value_end = strchr(ip, '&');
    if (value_end != NULL) {
         
        return apr_pstrndup(pool, ip, value_end - ip);
    } else {
         
        return apr_pstrdup(pool, ip);
    }
}
