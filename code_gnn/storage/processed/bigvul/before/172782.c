void impeg2d_bit_stream_init(stream_t *ps_stream,
                             UWORD8 *pu1_byte_buf,
                             UWORD32 u4_max_offset)
{
    UWORD8      *pu1_byte_buff;
    UWORD32     *pu4_word_buf;
 size_t     u4_byte_addr;
    UWORD32     u4_temp1,u4_temp2;

  
    ps_stream->pv_bs_buf           = pu1_byte_buf;
    ps_stream->u4_offset              = 0;

  
    pu1_byte_buff               = (UWORD8 *)pu1_byte_buf;
    u4_byte_addr                = (size_t)pu1_byte_buff;

 if((u4_byte_addr & 3) == 1)
 {
        u4_temp1                = ((UWORD32)(*pu1_byte_buff++)) << 8;
        u4_temp1                += ((UWORD32)(*pu1_byte_buff++)) << 16;
        u4_temp1                += ((UWORD32)(*pu1_byte_buff++)) << 24;

        pu4_word_buf            = (UWORD32 *)pu1_byte_buff;

        ps_stream->u4_offset          = 8;
 }
 else if((u4_byte_addr & 3) == 2)
 {
        u4_temp1                = ((UWORD32)(*pu1_byte_buff++)) << 16;
        u4_temp1                += ((UWORD32)(*pu1_byte_buff++)) << 24;

        pu4_word_buf            = (UWORD32 *)pu1_byte_buff;

        ps_stream->u4_offset          = 16;
 }
 else if((u4_byte_addr & 3) == 3)
 {
        u4_temp1                = (((UWORD32)(*pu1_byte_buff++)) << 24);

        pu4_word_buf            = (UWORD32 *)pu1_byte_buff;

        ps_stream->u4_offset          = 24;
 }
 else
 {
        pu4_word_buf            = (UWORD32 *)pu1_byte_buff;

        u4_temp1                = *pu4_word_buf++;
        ps_stream->u4_offset          = 0;
 }

  
    CONV_LE_TO_BE(u4_temp2,u4_temp1)

  
    u4_temp1                    = *pu4_word_buf++;
    ps_stream->u4_buf              = u4_temp2;

    CONV_LE_TO_BE(u4_temp2,u4_temp1)

    ps_stream->u4_buf_nxt          = u4_temp2;

    ps_stream->pu4_buf_aligned      = pu4_word_buf;


    ps_stream->u4_max_offset        = (u4_max_offset << 3) + ps_stream->u4_offset;

 return;
}
