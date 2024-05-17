int QDECL Com_strCompare( const void *a, const void *b )
{
    const char **pa = (const char **)a;
    const char **pb = (const char **)b;
    return strcmp( *pa, *pb );
}
