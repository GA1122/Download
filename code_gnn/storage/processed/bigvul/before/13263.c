c_pdf14trans_write_ctm(byte **ppbuf, const gs_pdf14trans_params_t *pparams)
{
     
    byte *pbuf = *ppbuf;
    int len, code;

    len = cmd_write_ctm_return_length_nodevice(&pparams->ctm);
    pbuf--;  
    code = cmd_write_ctm(&pparams->ctm, pbuf, len);
    if (code < 0)
        return code;
    pbuf += len + 1;
    *ppbuf = pbuf;
    return 0;
}
