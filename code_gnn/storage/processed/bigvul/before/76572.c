static bool is_good_origin(const char *origin, const char *require)
{
    size_t origin_len  = strlen(origin);
    size_t require_len = strlen(require);
    if (origin_len < require_len)
        return false;
    if (memcmp(origin + (origin_len - require_len), require, require_len) != 0)
        return false;
    return true;
}
