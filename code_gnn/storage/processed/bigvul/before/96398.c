int load_abrt_conf_file(const char *file, map_string_t *settings)
{
    static const char *const base_directories[] = { DEFAULT_CONF_DIR, CONF_DIR, NULL };

    return load_conf_file_from_dirs(file, base_directories, settings,   false);
}