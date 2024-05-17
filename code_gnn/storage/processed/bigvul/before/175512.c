void ihevcd_copy_pps(codec_t *ps_codec, WORD32 pps_id, WORD32 pps_id_ref)
{
 pps_t *ps_pps, *ps_pps_ref;
    WORD16 *pi2_scaling_mat_backup;
    WORD32 scaling_mat_size;
 tile_t *ps_tile_backup;
    WORD32 max_tile_cols, max_tile_rows;
    WORD32 wd, ht;
    wd = ALIGN64(ps_codec->i4_wd);
    ht = ALIGN64(ps_codec->i4_ht);

    SCALING_MAT_SIZE(scaling_mat_size);
    max_tile_cols = (wd + MIN_TILE_WD - 1) / MIN_TILE_WD;
    max_tile_rows = (ht + MIN_TILE_HT - 1) / MIN_TILE_HT;

    ps_pps_ref = ps_codec->ps_pps_base + pps_id_ref;
    ps_pps = ps_codec->ps_pps_base + pps_id;

    pi2_scaling_mat_backup = ps_pps->pi2_scaling_mat;
    ps_tile_backup = ps_pps->ps_tile;

    memcpy(ps_pps, ps_pps_ref, sizeof(pps_t));
    ps_pps->pi2_scaling_mat = pi2_scaling_mat_backup;
    ps_pps->ps_tile = ps_tile_backup;
    memcpy(ps_pps->pi2_scaling_mat, ps_pps_ref->pi2_scaling_mat, scaling_mat_size * sizeof(WORD16));
    memcpy(ps_pps->ps_tile, ps_pps_ref->ps_tile, max_tile_cols * max_tile_rows * sizeof(tile_t));

    ps_pps->i1_pps_valid = 1;

    ps_codec->s_parse.ps_pps = ps_pps;
}
