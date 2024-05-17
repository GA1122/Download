struct host *mk_vhost_read(char *path)
{
    unsigned long len = 0;
    char *tmp;
    char *host_low;
    struct stat checkdir;
    struct host *host;
    struct host_alias *new_alias;
    struct error_page *err_page;
    struct mk_config *cnf;
    struct mk_config_section *section_host;
    struct mk_config_section *section_ep;
    struct mk_config_entry *entry_ep;
    struct mk_string_line *entry;
    struct mk_list *head, *list;

     
    cnf = mk_config_create(path);
    if (!cnf) {
        mk_err("Configuration error, aborting.");
        exit(EXIT_FAILURE);
    }

     
    section_host = mk_config_section_get(cnf, "HOST");
    if (!section_host) {
        mk_err("Invalid config file %s", path);
        return NULL;
    }

     
    host = mk_mem_malloc_z(sizeof(struct host));
    host->config = cnf;
    host->file = mk_string_dup(path);

     
    mk_list_init(&host->error_pages);

     
    mk_list_init(&host->server_names);

     
    list = mk_config_section_getval(section_host, "Servername", MK_CONFIG_VAL_LIST);
    if (!list) {
        mk_err("Hostname does not contain a Servername");
        exit(EXIT_FAILURE);
    }

    mk_list_foreach(head, list) {
        entry = mk_list_entry(head, struct mk_string_line, _head);
        if (entry->len > MK_HOSTNAME_LEN - 1) {
            continue;
        }

         
        host_low = mk_string_tolower(entry->val);

         
        new_alias = mk_mem_malloc_z(sizeof(struct host_alias));
        new_alias->name = mk_mem_malloc_z(entry->len + 1);
        strncpy(new_alias->name, host_low, entry->len);
        mk_mem_free(host_low);

        new_alias->len = entry->len;

        mk_list_add(&new_alias->_head, &host->server_names);
    }
    mk_string_split_free(list);

     
    host->documentroot.data = mk_config_section_getval(section_host,
                                                       "DocumentRoot",
                                                       MK_CONFIG_VAL_STR);
    if (!host->documentroot.data) {
        mk_err("Missing DocumentRoot entry on %s file", path);
        mk_config_free(cnf);
        return NULL;
    }

    host->documentroot.len = strlen(host->documentroot.data);

     
    if (stat(host->documentroot.data, &checkdir) == -1) {
        mk_err("Invalid path to DocumentRoot in %s", path);
    }
    else if (!(checkdir.st_mode & S_IFDIR)) {
        mk_err("DocumentRoot variable in %s has an invalid directory path", path);
    }

    if (mk_list_is_empty(&host->server_names) == 0) {
        mk_config_free(cnf);
        return NULL;
    }

     
    host->header_redirect.data = NULL;
    host->header_redirect.len  = 0;

    tmp = mk_config_section_getval(section_host,
                                   "Redirect",
                                   MK_CONFIG_VAL_STR);
    if (tmp) {
        host->header_redirect.data = mk_string_dup(tmp);
        host->header_redirect.len  = strlen(tmp);
        mk_mem_free(tmp);
    }

     
    section_ep = mk_config_section_get(cnf, "ERROR_PAGES");
    if (section_ep) {
        mk_list_foreach(head, &section_ep->entries) {
            entry_ep = mk_list_entry(head, struct mk_config_entry, _head);

            int ep_status = -1;
            char *ep_file = NULL;
            unsigned long len;

            ep_status = atoi(entry_ep->key);
            ep_file   = entry_ep->val;

             
            if (ep_status < MK_CLIENT_BAD_REQUEST ||
                ep_status > MK_SERVER_HTTP_VERSION_UNSUP ||
                ep_file == NULL) {
                continue;
            }

             
            err_page = mk_mem_malloc_z(sizeof(struct error_page));
            err_page->status = ep_status;
            err_page->file   = mk_string_dup(ep_file);
            err_page->real_path = NULL;
            mk_string_build(&err_page->real_path, &len, "%s/%s",
                            host->documentroot.data, err_page->file);

            MK_TRACE("Map error page: status %i -> %s", err_page->status, err_page->file);

             
            mk_list_add(&err_page->_head, &host->error_pages);
        }
    }

     
    if (config->hideversion == MK_FALSE) {
        mk_string_build(&host->host_signature, &len,
                        "Monkey/%s", VERSION);
    }
    else {
        mk_string_build(&host->host_signature, &len, "Monkey");
    }
    mk_string_build(&host->header_host_signature.data,
                    &host->header_host_signature.len,
                    "Server: %s", host->host_signature);

    return host;
}