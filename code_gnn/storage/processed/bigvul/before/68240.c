read_object (FILE *in)
{
    Attr *attr = NULL;

     
    int tmp_char = fgetc(in);
    if (tmp_char == -1) return NULL;
    ungetc(tmp_char, in);

    attr = attr_read (in);

    return attr;
}
