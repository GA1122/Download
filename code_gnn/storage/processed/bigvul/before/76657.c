static void t1_outhex(byte b)
{
    static const char *hexdigits = "0123456789ABCDEF";
    t1_putchar(hexdigits[b/16]);
    t1_putchar(hexdigits[b%16]);
    hexline_length += 2;
    end_hexline();
}
