static void ssh_fix_verstring(char *str)
{
     
    while (*str && *str != '-') str++;
    assert(*str == '-'); str++;

     
    while (*str) {
        if (*str == '-' || *str == ' ')
            *str = '_';
        str++;
    }
}
