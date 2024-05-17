INLINE UWORD32 impeg2d_bit_stream_get(void* pv_ctxt, UWORD32 u4_num_bits)
{
    UWORD32 u4_next_bits = impeg2d_bit_stream_nxt(pv_ctxt, u4_num_bits);
    impeg2d_bit_stream_flush(pv_ctxt, u4_num_bits);
 return(u4_next_bits);
}
