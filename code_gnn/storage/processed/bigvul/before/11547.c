bool is_device_path(const char *path) {

         

        return
                path_startswith(path, "/dev/") ||
                path_startswith(path, "/sys/");
}
