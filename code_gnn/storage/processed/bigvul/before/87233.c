free_res_descriptor(RES_DESCRIPTOR * rd)
{
    RES_DESCRIPTOR *nxt;

    if (rd == NULL)
        return;
    nxt = rd->next;
    if (rd->name != NULL)
        free(rd->name);
    free(rd);
    free_res_descriptor(nxt);    
}