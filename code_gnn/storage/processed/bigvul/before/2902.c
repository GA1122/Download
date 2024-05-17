gs_main_lib_open(gs_main_instance * minst, const char *file_name, ref * pfile)
{
     
     
    i_ctx_t *i_ctx_p = minst->i_ctx_p;
#define maxfn 2048
    char fn[maxfn];
    uint len;

    return lib_file_open(&minst->lib_path, imemory,
                         NULL,  
                          file_name, strlen(file_name), fn, maxfn, &len, pfile);
}
