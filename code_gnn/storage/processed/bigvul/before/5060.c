CompareEntries(const void *e1, const void *e2)
{
    return strcmp(((Entry *)e1)->tag, ((Entry *)e2)->tag);
}
