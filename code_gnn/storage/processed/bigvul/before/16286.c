void mt_init() {
    int i;
    srand(time(NULL));
    for (i = 0; i < MT_LEN; i++)
        mt_buffer[i] = rand();
    mt_index = 0;
}
