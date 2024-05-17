IHEVCD_ERROR_T ihevcd_operation_point_set(vps_t *ps_vps, bitstrm_t *ps_bitstrm, WORD32 ops_idx)
{
    WORD32 i;
    WORD32 value;

    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
    UNUSED(ops_idx);
 for(i = 0; i <= ps_vps->i1_vps_max_nuh_reserved_zero_layer_id; i++)
 {
        BITS_PARSE("list_entry_l0[ i ]", value, ps_bitstrm, 1);

 }
    UNUSED(value);

 return ret;
}
