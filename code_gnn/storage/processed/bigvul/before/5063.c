FreeEntries(Entries *e)
{
    register int i;

    for (i = 0; i < e->used; i++) {
	if (e->entry[i].usable) {
	    free(e->entry[i].tag);
	    free(e->entry[i].value);
	}
    }
    free((char *)e->entry);
}
