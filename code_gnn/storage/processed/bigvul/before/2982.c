rc_pdf14_maskbuf_free(gs_memory_t * mem, void *ptr_in, client_name_t cname)
{
     
    pdf14_rcmask_t *rcmask = (pdf14_rcmask_t * ) ptr_in;
     
    if ( rcmask->mask_buf != NULL ){
        pdf14_buf_free(rcmask->mask_buf);
    }
    gs_free_object(mem, rcmask, "rc_pdf14_maskbuf_free");
}
