bool param_defined(const char* name) {
    char* v = param_without_default(name);
    if (NULL == v) return false;
    free(v);
    return true;
}
