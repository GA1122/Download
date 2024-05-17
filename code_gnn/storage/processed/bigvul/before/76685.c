static void enc_getline(void)
{
    char *p;
    char c;
  restart:
    if (enc_eof())
        normal_error("type 1","unexpected end of file");
    p = enc_line;
    do {
        c = (char) enc_getchar();
        append_char_to_buf(c, p, enc_line, ENC_BUF_SIZE);
    }
    while (c != 10 && !enc_eof());
    append_eol(p, enc_line, ENC_BUF_SIZE);
    if (p - enc_line < 2 || *enc_line == '%')
        goto restart;
}
