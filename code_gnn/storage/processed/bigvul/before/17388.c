XineramaDeleteResource(void *data, XID id)
{
    free(data);
    return 1;
}
