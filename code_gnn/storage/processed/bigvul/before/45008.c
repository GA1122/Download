static int r_flush(apr_vformatter_buff_t *buff)
{
     

     
    struct ap_vrprintf_data *vd = (struct ap_vrprintf_data*)buff;

    if (vd->r->connection->aborted)
        return -1;

     
    if (buffer_output(vd->r, vd->buff, AP_IOBUFSIZE)) {
        return -1;
    }

     
    vd->vbuff.curpos = vd->buff;
    vd->vbuff.endpos = vd->buff + AP_IOBUFSIZE;

    return 0;
}
