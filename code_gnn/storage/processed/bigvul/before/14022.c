int outwrite(const gs_memory_t *mem, const char *str, int len)
{
    int code;
    FILE *fout;
    gs_lib_ctx_t *pio = mem->gs_lib_ctx;

    if (len == 0)
        return 0;
    if (pio->stdout_is_redirected) {
        if (pio->stdout_to_stderr)
            return errwrite(mem, str, len);
        fout = pio->fstdout2;
    }
    else if (pio->stdout_fn) {
        return (*pio->stdout_fn)(pio->caller_handle, str, len);
    }
    else {
        fout = pio->fstdout;
    }
    code = fwrite(str, 1, len, fout);
    fflush(fout);
    return code;
}
