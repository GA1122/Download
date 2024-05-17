ResFindAllRes(void *value, XID id, RESTYPE type, void *cdata)
{
    int *counts = (int *) cdata;

    counts[(type & TypeMask) - 1]++;
}
