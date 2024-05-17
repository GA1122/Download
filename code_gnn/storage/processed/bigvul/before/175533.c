void ihevcd_unmark_pps(codec_t *ps_codec, WORD32 sps_id)
{
    WORD32 pps_id = 0;
 pps_t *ps_pps = ps_codec->ps_pps_base;

 for(pps_id = 0; pps_id < MAX_PPS_CNT - 1; pps_id++, ps_pps++)
 {
 if((ps_pps->i1_pps_valid) &&
 (ps_pps->i1_sps_id == sps_id))
            ps_pps->i1_pps_valid = 0;
 }
}
