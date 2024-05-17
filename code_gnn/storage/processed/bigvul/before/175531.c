IHEVCD_ERROR_T  ihevcd_scaling_list_data(codec_t *ps_codec, WORD16 *pi2_scaling_mat)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
    WORD32 size_id;
    WORD32 matrix_id;
    WORD32 value, dc_value = 0;
    WORD32 next_coef;
    WORD32 coef_num;
    WORD32 i, j, offset;
 bitstrm_t *ps_bitstrm = &ps_codec->s_parse.s_bitstrm;
    WORD16 *pi2_scaling_mat_offset;
    WORD32 scaling_mat_offset[] = { 0, 16, 32, 48, 64, 80, 96, 160, 224, 288, 352, 416, 480, 736, 992, 1248, 1504, 1760, 2016, 3040 };
    UWORD8 *scan_table;

 for(size_id = 0; size_id < 4; size_id++)
 {
 for(matrix_id = 0; matrix_id < ((size_id == 3) ? 2 : 6); matrix_id++)
 {
            WORD32 scaling_list_pred_mode_flag;
            WORD32 scaling_list_delta_coef;
            BITS_PARSE("scaling_list_pred_mode_flag", scaling_list_pred_mode_flag, ps_bitstrm, 1);

            offset = size_id * 6 + matrix_id;
            pi2_scaling_mat_offset = pi2_scaling_mat + scaling_mat_offset[offset];

 if(!scaling_list_pred_mode_flag)
 {
                WORD32 num_elements;
                UEV_PARSE("scaling_list_pred_matrix_id_delta", value,
                          ps_bitstrm);
                value = CLIP3(value, 0, matrix_id);

                num_elements = (1 << (4 + (size_id << 1)));
 if(0 != value)
                    memcpy(pi2_scaling_mat_offset, pi2_scaling_mat_offset - value * num_elements, num_elements * sizeof(WORD16));
 }
 else
 {
                next_coef = 8;
                coef_num = MIN(64, (1 << (4 + (size_id << 1))));

 if(size_id > 1)
 {
                    SEV_PARSE("scaling_list_dc_coef_minus8", value,
                              ps_bitstrm);

                    next_coef = value + 8;
                    dc_value = next_coef;
 }
 if(size_id < 2)
 {
                    scan_table = (UWORD8 *)gapv_ihevc_invscan[size_id + 1];

 for(i = 0; i < coef_num; i++)
 {
                        SEV_PARSE("scaling_list_delta_coef",
                                  scaling_list_delta_coef, ps_bitstrm);
                        next_coef = (next_coef + scaling_list_delta_coef + 256)
 % 256;
                        pi2_scaling_mat_offset[scan_table[i]] = next_coef;
 }
 }
 else if(size_id == 2)
 {
                    scan_table = (UWORD8 *)gapv_ihevc_invscan[2];

 for(i = 0; i < coef_num; i++)
 {
                        SEV_PARSE("scaling_list_delta_coef",
                                  scaling_list_delta_coef, ps_bitstrm);
                        next_coef = (next_coef + scaling_list_delta_coef + 256)
 % 256;

                        offset = scan_table[i];
                        offset = (offset >> 3) * 16 * 2 + (offset & 0x7) * 2;
                        pi2_scaling_mat_offset[offset] = next_coef;
                        pi2_scaling_mat_offset[offset + 1] = next_coef;
                        pi2_scaling_mat_offset[offset + 16] = next_coef;
                        pi2_scaling_mat_offset[offset + 16 + 1] = next_coef;
 }
                    pi2_scaling_mat_offset[0] = dc_value;
 }
 else
 {
                    scan_table = (UWORD8 *)gapv_ihevc_invscan[2];

 for(i = 0; i < coef_num; i++)
 {
                        SEV_PARSE("scaling_list_delta_coef",
                                  scaling_list_delta_coef, ps_bitstrm);
                        next_coef = (next_coef + scaling_list_delta_coef + 256)
 % 256;

                        offset = scan_table[i];
                        offset = (offset >> 3) * 32 * 4 + (offset & 0x7) * 4;

 for(j = 0; j < 4; j++)
 {
                            pi2_scaling_mat_offset[offset + j * 32] = next_coef;
                            pi2_scaling_mat_offset[offset + 1 + j * 32] = next_coef;
                            pi2_scaling_mat_offset[offset + 2 + j * 32] = next_coef;
                            pi2_scaling_mat_offset[offset + 3 + j * 32] = next_coef;
 }
                        pi2_scaling_mat_offset[0] = dc_value;
 }
 }
 }
 }
 }

 return ret;
}
