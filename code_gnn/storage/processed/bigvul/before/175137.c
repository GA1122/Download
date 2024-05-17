wifi_error wifi_init_interfaces(wifi_handle handle)
{
    hal_info *info = (hal_info *)handle;

 struct dirent *de;

 DIR *d = opendir("/sys/class/net");
 if (d == 0)
 return WIFI_ERROR_UNKNOWN;

 int n = 0;
 while ((de = readdir(d))) {
 if (de->d_name[0] == '.')
 continue;
 if (is_wifi_interface(de->d_name) ) {
            n++;
 }
 }

    closedir(d);

    d = opendir("/sys/class/net");
 if (d == 0)
 return WIFI_ERROR_UNKNOWN;

    info->interfaces = (interface_info **)malloc(sizeof(interface_info *) * n);

 int i = 0;
 while ((de = readdir(d))) {
 if (de->d_name[0] == '.')
 continue;
 if (is_wifi_interface(de->d_name)) {
            interface_info *ifinfo = (interface_info *)malloc(sizeof(interface_info));
 if (get_interface(de->d_name, ifinfo) != WIFI_SUCCESS) {
                free(ifinfo);
 continue;
 }
            ifinfo->handle = handle;
            info->interfaces[i] = ifinfo;
            i++;
 }
 }

    closedir(d);

    info->num_interfaces = n;
 return WIFI_SUCCESS;
}
