file_close(ref * pfile)
{
    stream *s;

    if (file_is_valid(s, pfile)) {       
        if (sclose(s))
            return_error(gs_error_ioerror);
    }
    return 0;
}
