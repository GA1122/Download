void mk_vhost_init(char *path)
{
    DIR *dir;
    unsigned long len;
    char *buf = 0;
    char *sites = 0;
    char *file;
    struct host *p_host;      
    struct dirent *ent;
    struct file_info f_info;
    int ret;

     
    mk_string_build(&sites, &len, "%s/%s/", path, config->sites_conf_dir);
    ret = mk_file_get_info(sites, &f_info);
    if (ret == -1 || f_info.is_directory == MK_FALSE) {
        mk_mem_free(sites);
        sites = config->sites_conf_dir;
    }

    mk_string_build(&buf, &len, "%s/default", sites);

    p_host = mk_vhost_read(buf);
    if (!p_host) {
        mk_err("Error parsing main configuration file 'default'");
    }
    mk_list_add(&p_host->_head, &config->hosts);
    config->nhosts++;
    mk_mem_free(buf);
    buf = NULL;


     
    if (!(dir = opendir(sites))) {
        mk_mem_free(sites);
        mk_err("Could not open %s", sites);
        exit(EXIT_FAILURE);
    }

     
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_name[0] == '.') {
            continue;
        }
        if (strcmp((char *) ent->d_name, "..") == 0) {
            continue;
        }
        if (ent->d_name[strlen(ent->d_name) - 1] ==  '~') {
            continue;
        }
        if (strcasecmp((char *) ent->d_name, "default") == 0) {
            continue;
        }
        file = NULL;
        mk_string_build(&file, &len, "%s/%s", sites, ent->d_name);

        p_host = mk_vhost_read(file);
        mk_mem_free(file);
        if (!p_host) {
            continue;
        }
        else {
            mk_list_add(&p_host->_head, &config->hosts);
            config->nhosts++;
        }
    }
    closedir(dir);
    mk_mem_free(sites);
}