RAND_DRBG *RAND_DRBG_new(int type, unsigned int flags, RAND_DRBG *parent)
{
    return rand_drbg_new(0, type, flags, parent);
}
