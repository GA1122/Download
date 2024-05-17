void mk_vhost_free_all()
{
    struct host *host;
    struct host_alias *host_alias;
    struct error_page *ep;
    struct mk_list *head_host;
    struct mk_list *head_alias;
    struct mk_list *head_error;
    struct mk_list *tmp1, *tmp2;

    mk_list_foreach_safe(head_host, tmp1, &config->hosts) {
        host = mk_list_entry(head_host, struct host, _head);
        mk_list_del(&host->_head);

        mk_mem_free(host->file);

         
        mk_list_foreach_safe(head_alias, tmp2, &host->server_names) {
            host_alias = mk_list_entry(head_alias, struct host_alias, _head);
            mk_list_del(&host_alias->_head);
            mk_mem_free(host_alias->name);
            mk_mem_free(host_alias);
        }

         
        mk_list_foreach_safe(head_error, tmp2, &host->error_pages) {
            ep = mk_list_entry(head_error, struct error_page, _head);
            mk_list_del(&ep->_head);
            mk_mem_free(ep->file);
            mk_mem_free(ep->real_path);
            mk_mem_free(ep);
        }

        mk_ptr_free(&host->documentroot);
        mk_mem_free(host->host_signature);
        mk_ptr_free(&host->header_host_signature);

         
        if (host->config) mk_config_free(host->config);
        mk_mem_free(host);
    }
}