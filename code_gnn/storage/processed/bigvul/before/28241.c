static int get_consumed_bytes(int pos, int buf_size)
{
    if (pos == 0)
        pos = 1;           
    if (pos + 10 > buf_size)
        pos = buf_size;                    

    return pos;
}
