INLINE UWORD8 impeg2d_bit_stream_get_bit(stream_t *ps_stream)
{
    UWORD32     u4_bit,u4_offset,u4_temp;
    UWORD32     u4_curr_bit;

    u4_offset               = ps_stream->u4_offset;
    u4_curr_bit             = u4_offset & 0x1F;
    u4_bit                  = ps_stream->u4_buf;

  
    u4_bit                  >>= BITS_IN_INT - u4_curr_bit - 1;

    u4_offset++;

  
 if (u4_curr_bit == 31)
 {
        ps_stream->u4_buf      = ps_stream->u4_buf_nxt;

 if (ps_stream->u4_offset < ps_stream->u4_max_offset)
 {
            u4_temp             = *(ps_stream->pu4_buf_aligned)++;
            CONV_LE_TO_BE(ps_stream->u4_buf_nxt,u4_temp)
 }
 }
    ps_stream->u4_offset          = u4_offset;

 return (u4_bit & 0x1);
}
