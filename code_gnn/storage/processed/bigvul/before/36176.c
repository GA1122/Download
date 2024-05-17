void mk_vhost_set_single(char *path)
{
    struct host *host;
    struct host_alias *halias;
    struct stat checkdir;
    unsigned long len = 0;

     
    host = mk_mem_malloc_z(sizeof(struct host));
    mk_list_init(&host->error_pages);
    mk_list_init(&host->server_names);

     
    halias = mk_mem_malloc_z(sizeof(struct host_alias));
    halias->name = mk_string_dup("127.0.0.1");
    mk_list_add(&halias->_head, &host->server_names);

    host->documentroot.data = mk_string_dup(path);
    host->documentroot.len = strlen(path);
    host->header_redirect.data = NULL;

     
    if (stat(host->documentroot.data, &checkdir) == -1) {
        mk_err("Invalid path to DocumentRoot in %s", path);
        exit(EXIT_FAILURE);
    }
    else if (!(checkdir.st_mode & S_IFDIR)) {
        mk_err("DocumentRoot variable in %s has an invalid directory path", path);
        exit(EXIT_FAILURE);
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

    mk_list_add(&host->_head, &config->hosts);
}