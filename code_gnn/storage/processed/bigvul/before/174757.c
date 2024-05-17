UWORD8 impeg2d_inv_quant_mpeg1(WORD16 *pi2_blk,
                              UWORD8 *pu1_weighting_matrix,
                              UWORD8 u1_quant_scale,
                              WORD32 u4_intra_flag,
                              WORD32 i4_num_coeffs,
                              WORD16 *pi2_coeffs,
                              UWORD8 *pu1_pos,
 const UWORD8 *pu1_scan,
                              UWORD16 *pu2_def_dc_pred,
                              UWORD16 u2_intra_dc_precision)
{
    UWORD16 i4_pos;

    WORD32  i4_iter;

  
 if(u4_intra_flag == 1)
 {
  
  
  
  
 {
            pi2_coeffs[0] += *pu2_def_dc_pred;
 *pu2_def_dc_pred      = pi2_coeffs[0];
            pi2_coeffs[0] <<= (3 - u2_intra_dc_precision);
            pi2_coeffs[0] = CLIP_S12(pi2_coeffs[0]);
 }

        pi2_blk[pu1_scan[0]] = pi2_coeffs[0];
 }
  
  
  
 for(i4_iter = u4_intra_flag; i4_iter < i4_num_coeffs; i4_iter++)
 {

        WORD16 sign;
        WORD32 temp, temp1;

  
        i4_pos      = pu1_pos[i4_iter];
        pi2_blk[i4_pos] = pi2_coeffs[i4_iter];

        sign = SIGN(pi2_blk[i4_pos]);
        temp = ABS(pi2_blk[i4_pos] << 1);

  
        temp = temp + (1 * !u4_intra_flag);

        temp = temp * pu1_weighting_matrix[i4_pos] * u1_quant_scale;

        temp = temp >> 5;

        temp1 = temp | 1;

        temp1 = (temp1 > temp) ? (temp1 - temp) : (temp - temp1);

        temp = temp - temp1;

 if(temp < 0)
 {
            temp = 0;
 }

        temp = temp * sign;

        temp = CLIP_S12(temp);

        pi2_blk[i4_pos] = temp;
 }

  
 return (0);
}  
