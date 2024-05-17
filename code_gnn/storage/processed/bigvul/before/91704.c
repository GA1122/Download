const char *am_filepath_dirname(apr_pool_t *p, const char *path) 
{
    char *cp;

     
    if (((cp = strrchr(path, (int)'/')) == NULL) &&
        ((cp = strrchr(path, (int)'\\')) == NULL))
            return ".";
   
    return apr_pstrndup(p, path, cp - path);
}
