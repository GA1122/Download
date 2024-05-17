void impeg2d_dec_quant_matrix_ext(dec_state_t *ps_dec)
{
 stream_t *ps_stream;

    ps_stream = &ps_dec->s_bit_stream;
  
    impeg2d_bit_stream_flush(ps_stream,4);

  
  
  
 if(impeg2d_bit_stream_get(ps_stream,1) == 1)
 {
        UWORD16 i;
 for(i = 0; i < NUM_PELS_IN_BLOCK; i++)
 {
            ps_dec->au1_intra_quant_matrix[gau1_impeg2_inv_scan_zig_zag[i]] = (UWORD8)impeg2d_bit_stream_get(ps_stream,8);
 }

 }


  
  
  
 if(impeg2d_bit_stream_get(ps_stream,1) == 1)
 {
        UWORD16 i;
 for(i = 0; i < NUM_PELS_IN_BLOCK; i++)
 {
            ps_dec->au1_inter_quant_matrix[gau1_impeg2_inv_scan_zig_zag[i]] = (UWORD8)impeg2d_bit_stream_get(ps_stream,8);
 }
 }

  
    impeg2d_next_start_code(ps_dec);
}
