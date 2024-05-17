VerifyDisplayName(const char *d)
{
    if ( d == (char *)0 ) return 0;   
    if ( *d == '\0' ) return 0;   
    if ( *d == '-' ) return 0;   
    if ( *d == '.' ) return 0;   
    if ( strchr(d, '/') != (char *)0 ) return 0;   
    return 1;
}
