WORD32 ih264d_deblock_display(dec_struct_t *ps_dec)
{
    WORD32 ret;
  
    ih264d_deblock_picture(ps_dec);

    ret = ih264d_end_of_pic_dispbuf_mgr(ps_dec);
 if(ret != OK)
 return ret;

 return OK;
}
