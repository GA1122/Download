do_mkdir(const char *dir, long mode)
{
    char *r, abs[_MAX_PATH];
    size_t n;

    _abspath(abs, rc_dir, _MAX_PATH);	 

    if (!(n = strlen(abs)))
	return -1;

    if (*(r = abs + n - 1) == '/')	 
	*r = 0;

    return mkdir(abs, mode);
}