INLINE void impeg2d_bit_stream_flush_to_byte_boundary(void* pv_ctxt)
{
    UWORD8 u1_bit_offset;
 stream_t *ps_stream = (stream_t *)pv_ctxt;

    u1_bit_offset = (ps_stream->u4_offset) & 0x7;


  
 if(u1_bit_offset != 0)
 {
        impeg2d_bit_stream_flush(ps_stream,(8 - u1_bit_offset));
 }



}
