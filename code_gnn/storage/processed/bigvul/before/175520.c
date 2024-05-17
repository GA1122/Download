static IHEVCD_ERROR_T ihevcd_parse_profile_tier_level_layer(bitstrm_t *ps_bitstrm,
 profile_tier_lvl_t *ps_ptl)
{
    WORD32 value;
    WORD32 i;
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;

    BITS_PARSE("XXX_profile_space[]", value, ps_bitstrm, 2);
    ps_ptl->i1_profile_space = value;

    BITS_PARSE("XXX_tier_flag[]", value, ps_bitstrm, 1);
    ps_ptl->i1_tier_flag = value;

    BITS_PARSE("XXX_profile_idc[]", value, ps_bitstrm, 5);
    ps_ptl->i1_profile_idc = value;

 for(i = 0; i < MAX_PROFILE_COMPATBLTY; i++)
 {
        BITS_PARSE("XXX_profile_compatibility_flag[][j]", value, ps_bitstrm, 1);
        ps_ptl->ai1_profile_compatibility_flag[i] = value;
 }

    BITS_PARSE("general_progressive_source_flag", value, ps_bitstrm, 1);
    ps_ptl->i1_general_progressive_source_flag = value;

    BITS_PARSE("general_interlaced_source_flag", value, ps_bitstrm, 1);
    ps_ptl->i1_general_interlaced_source_flag = value;

    BITS_PARSE("general_non_packed_constraint_flag", value, ps_bitstrm, 1);
    ps_ptl->i1_general_non_packed_constraint_flag = value;

    BITS_PARSE("general_frame_only_constraint_flag", value, ps_bitstrm, 1);
    ps_ptl->i1_frame_only_constraint_flag = value;

    BITS_PARSE("XXX_reserved_zero_44bits[0..15]", value, ps_bitstrm, 16);

    BITS_PARSE("XXX_reserved_zero_44bits[16..31]", value, ps_bitstrm, 16);

    BITS_PARSE("XXX_reserved_zero_44bits[32..43]", value, ps_bitstrm, 12);
 return ret;
}
