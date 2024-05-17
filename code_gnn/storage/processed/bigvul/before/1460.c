static int get_dotl_openflags(V9fsState *s, int oflags)
{
    int flags;
     
    flags = dotl_to_open_flags(oflags);
    flags &= ~(O_NOCTTY | O_ASYNC | O_CREAT);
     
    flags &= ~O_DIRECT;
    return flags;
}
