static void _dlist_clean(struct dlist *dl)
{
    if (!dl) return;

     
    _dlist_free_children(dl);

     
    free(dl->part);
    dl->part = NULL;
    free(dl->sval);
    dl->sval = NULL;
    free(dl->gval);
    dl->gval = NULL;
    dl->nval = 0;
}
