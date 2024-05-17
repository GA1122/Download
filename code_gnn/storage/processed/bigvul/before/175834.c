int vp8dx_references_buffer( VP8_COMMON *oci, int ref_frame )
{
 const MODE_INFO *mi = oci->mi;
 int mb_row, mb_col;

 for (mb_row = 0; mb_row < oci->mb_rows; mb_row++)
 {
 for (mb_col = 0; mb_col < oci->mb_cols; mb_col++,mi++)
 {
 if( mi->mbmi.ref_frame == ref_frame)
 return 1;
 }
        mi++;
 }
 return 0;

}
