int path_is_absolute(const char *path)
{
#ifdef _WIN32
     
    if (is_windows_drive(path) || is_windows_drive_prefix(path)) {
        return 1;
    }
    return (*path == '/' || *path == '\\');
#else
    return (*path == '/');
#endif
}