static void coroutine_fn virtfs_reset(V9fsPDU *pdu)
{
    V9fsState *s = pdu->s;
    V9fsFidState *fidp;

     
    while (s->fid_list) {
         
        fidp = s->fid_list;
        fidp->ref++;

         
        s->fid_list = fidp->next;
        fidp->clunked = 1;

        put_fid(pdu, fidp);
    }
}
