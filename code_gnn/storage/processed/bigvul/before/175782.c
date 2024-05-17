WORD32 ih264d_fill_num_mem_rec(void *pv_api_ip, void *pv_api_op)
{

 ih264d_fill_mem_rec_ip_t *ps_mem_q_ip;
 ih264d_fill_mem_rec_op_t *ps_mem_q_op;
    WORD32 level;
    UWORD32 num_reorder_frames;
    UWORD32 num_ref_frames;
    UWORD32 num_extra_disp_bufs;
    UWORD32 u4_dpb_size_num_frames;
 iv_mem_rec_t *memTab;

    UWORD32 chroma_format, u4_share_disp_buf;
    UWORD32 u4_total_num_mbs;
    UWORD32 luma_width, luma_width_in_mbs;
    UWORD32 luma_height, luma_height_in_mbs;
    UWORD32 max_dpb_size;

    ps_mem_q_ip = (ih264d_fill_mem_rec_ip_t *)pv_api_ip;
    ps_mem_q_op = (ih264d_fill_mem_rec_op_t *)pv_api_op;

 if(ps_mem_q_ip->s_ivd_fill_mem_rec_ip_t.u4_size
 > offsetof(ih264d_fill_mem_rec_ip_t, i4_level))
 {
        level = ps_mem_q_ip->i4_level;
 }
 else
 {
        level = H264_LEVEL_3_1;
 }

 if(ps_mem_q_ip->s_ivd_fill_mem_rec_ip_t.u4_size
 > offsetof(ih264d_fill_mem_rec_ip_t, u4_num_reorder_frames))
 {
        num_reorder_frames = ps_mem_q_ip->u4_num_reorder_frames;
 }
 else
 {
        num_reorder_frames = H264_MAX_REF_PICS;
 }

 if(ps_mem_q_ip->s_ivd_fill_mem_rec_ip_t.u4_size
 > offsetof(ih264d_fill_mem_rec_ip_t, u4_num_ref_frames))
 {
        num_ref_frames = ps_mem_q_ip->u4_num_ref_frames;
 }
 else
 {
        num_ref_frames = H264_MAX_REF_PICS;
 }

 if(ps_mem_q_ip->s_ivd_fill_mem_rec_ip_t.u4_size
 > offsetof(ih264d_fill_mem_rec_ip_t, u4_num_extra_disp_buf))
 {
        num_extra_disp_bufs = ps_mem_q_ip->u4_num_extra_disp_buf;
 }
 else
 {
        num_extra_disp_bufs = 0;
 }

 if(ps_mem_q_ip->s_ivd_fill_mem_rec_ip_t.u4_size
 > offsetof(ih264d_fill_mem_rec_ip_t, u4_share_disp_buf))
 {
#ifndef LOGO_EN
        u4_share_disp_buf = ps_mem_q_ip->u4_share_disp_buf;
#else
        u4_share_disp_buf = 0;
#endif
 }
 else
 {
        u4_share_disp_buf = 0;
 }

 if(ps_mem_q_ip->s_ivd_fill_mem_rec_ip_t.u4_size
 > offsetof(ih264d_fill_mem_rec_ip_t, e_output_format))
 {
        chroma_format = ps_mem_q_ip->e_output_format;
 }
 else
 {
        chroma_format = -1;
 }

 if((chroma_format != IV_YUV_420P) && (chroma_format != IV_YUV_420SP_UV)
 && (chroma_format != IV_YUV_420SP_VU))
 {
        u4_share_disp_buf = 0;
 }
 if(0 == u4_share_disp_buf)
        num_extra_disp_bufs = 0;

 {

        luma_height = ps_mem_q_ip->s_ivd_fill_mem_rec_ip_t.u4_max_frm_ht;
        luma_width = ps_mem_q_ip->s_ivd_fill_mem_rec_ip_t.u4_max_frm_wd;

        luma_height = ALIGN32(luma_height);
        luma_width = ALIGN16(luma_width);
        luma_width_in_mbs = luma_width >> 4;
        luma_height_in_mbs = luma_height >> 4;
        u4_total_num_mbs = (luma_height * luma_width) >> 8;
 }
  
 if(u4_total_num_mbs > MAX_MBS_LEVEL_30 && level < H264_LEVEL_3_1)
 {
        level = H264_LEVEL_3_1;
 }

 if((level < MIN_LEVEL_SUPPORTED) || (level > MAX_LEVEL_SUPPORTED))
 {
        ps_mem_q_op->s_ivd_fill_mem_rec_op_t.u4_error_code |=
                        ERROR_LEVEL_UNSUPPORTED;
 return (IV_FAIL);
 }

 if(num_ref_frames > H264_MAX_REF_PICS)
 {
        ps_mem_q_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= ERROR_NUM_REF;
        num_ref_frames = H264_MAX_REF_PICS;
 }

 if(num_reorder_frames > H264_MAX_REF_PICS)
 {
        ps_mem_q_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= ERROR_NUM_REF;
        num_reorder_frames = H264_MAX_REF_PICS;
 }
    memTab = ps_mem_q_ip->s_ivd_fill_mem_rec_ip_t.pv_mem_rec_location;

    memTab[MEM_REC_IV_OBJ].u4_mem_size = sizeof(iv_obj_t);
    memTab[MEM_REC_IV_OBJ].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_IV_OBJ].e_mem_type = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    H264_DEC_DEBUG_PRINT("MEM_REC_IV_OBJ MEM Size = %d\n",
                         memTab[MEM_REC_IV_OBJ].u4_mem_size);

    memTab[MEM_REC_CODEC].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_CODEC].e_mem_type = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    memTab[MEM_REC_CODEC].u4_mem_size = sizeof(dec_struct_t);

 {
        UWORD32 mvinfo_size, mv_info_size_pad;
        UWORD32 MVbank, MVbank_pad;
        UWORD32 Ysize;
        UWORD32 UVsize;
        UWORD32 one_frm_size;

        UWORD32 extra_mem = 0;

        UWORD32 pad_len_h, pad_len_v;

  
        UWORD32 num_buf;

 {
            UWORD32 num_bufs_app, num_bufs_level;

            num_bufs_app = num_ref_frames + num_reorder_frames + 1;

 if(num_bufs_app <= 1)
                num_bufs_app = 2;

            num_bufs_level = ih264d_get_dpb_size_new(level, (luma_width >> 4),
 (luma_height >> 4));

            max_dpb_size = num_bufs_level;

            num_bufs_level = num_bufs_level * 2 + 1;

            num_buf = MIN(num_bufs_level, num_bufs_app);

            num_buf += num_extra_disp_bufs;

 }

        mvinfo_size = ((luma_width * (luma_height)) >> 4);

        mv_info_size_pad = ((luma_width * (PAD_MV_BANK_ROW)) >> 4);

 Ysize = ALIGN32((luma_width + (PAD_LEN_Y_H << 1)))
 * (luma_height + (PAD_LEN_Y_V << 2));


 UVsize = Ysize >> 2;
 if(u4_share_disp_buf == 1)
 {
  

 if((chroma_format == IV_YUV_420P)
 || (chroma_format == IV_YUV_420SP_UV)
 || (chroma_format == IV_YUV_420SP_VU))
 {
 Ysize = 64;
 }
 if(chroma_format == IV_YUV_420SP_UV)
 {
 UVsize = 64;
 }
 }

        one_frm_size = (((Ysize + 127) >> 7) << 7)
 + ((((UVsize << 1) + 127) >> 7) << 7);


  
 MVbank = sizeof(mv_pred_t) * mvinfo_size;
 MVbank_pad = sizeof(mv_pred_t) * mv_info_size_pad;

 MVbank = (((MVbank + 127) >> 7) << 7);

 MVbank_pad = (((MVbank_pad + 127) >> 7) << 7);

        memTab[MEM_REC_MVBANK].u4_mem_alignment = (128 * 8) / CHAR_BIT;
        memTab[MEM_REC_MVBANK].e_mem_type =
                        IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        memTab[MEM_REC_MVBANK].u4_mem_size = (MVbank + MVbank_pad)
 * (MIN(max_dpb_size, num_ref_frames) + 1);


        memTab[MEM_REC_REF_PIC].u4_mem_alignment = (128 * 8) / CHAR_BIT;
        memTab[MEM_REC_REF_PIC].e_mem_type =
                        IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        memTab[MEM_REC_REF_PIC].u4_mem_size = one_frm_size * num_buf;

 }

    memTab[MEM_REC_DEBLK_MB_INFO].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_DEBLK_MB_INFO].e_mem_type =
                    IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    memTab[MEM_REC_DEBLK_MB_INFO].u4_mem_size = (((((u4_total_num_mbs
 + (luma_width >> 4)) * sizeof(deblk_mb_t)) + 127) >> 7) << 7);

    memTab[MEM_REC_NEIGHBOR_INFO].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_NEIGHBOR_INFO].e_mem_type =
                    IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    memTab[MEM_REC_NEIGHBOR_INFO].u4_mem_size = sizeof(mb_neigbour_params_t)
 * ((luma_width + 16) >> 4) * 2 * 2;
 {
        WORD32 size;
        WORD32 num_entries;

        num_entries = MIN(MAX_FRAMES, num_ref_frames);
        num_entries = 2 * ((2 * num_entries) + 1);

        size = num_entries * sizeof(void *);
        size += PAD_MAP_IDX_POC * sizeof(void *);
        size *= u4_total_num_mbs;
        size += sizeof(dec_slice_struct_t) * u4_total_num_mbs;
        memTab[MEM_REC_SLICE_HDR].u4_mem_alignment = (128 * 8) / CHAR_BIT;
        memTab[MEM_REC_SLICE_HDR].e_mem_type = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        memTab[MEM_REC_SLICE_HDR].u4_mem_size = size;
 }
 {

        UWORD32 u4_num_entries;

        u4_num_entries = u4_total_num_mbs;

        memTab[MEM_REC_MB_INFO].u4_mem_alignment = (128 * 8) / CHAR_BIT;
        memTab[MEM_REC_MB_INFO].e_mem_type =
                        IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        memTab[MEM_REC_MB_INFO].u4_mem_size = sizeof(dec_mb_info_t)
 * u4_num_entries;

        memTab[MEM_REC_PRED_INFO].u4_mem_alignment = (128 * 8) / CHAR_BIT;
        memTab[MEM_REC_PRED_INFO].e_mem_type =
                        IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;

        memTab[MEM_REC_PRED_INFO].u4_mem_size = sizeof(pred_info_t) * 2*32;

        memTab[MEM_REC_COEFF_DATA].u4_mem_alignment = (128 * 8) / CHAR_BIT;
        memTab[MEM_REC_COEFF_DATA].e_mem_type =
                        IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        memTab[MEM_REC_COEFF_DATA].u4_mem_size = MB_LUM_SIZE * sizeof(WORD16);
  
        memTab[MEM_REC_COEFF_DATA].u4_mem_size += u4_num_entries
 * (MAX(17 * sizeof(tu_sblk4x4_coeff_data_t),4 * sizeof(tu_blk8x8_coeff_data_t))
 + 9 * sizeof(tu_sblk4x4_coeff_data_t));
        memTab[MEM_REC_COEFF_DATA].u4_mem_size += u4_num_entries * 32;

 }

    memTab[MEM_REC_SPS].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_SPS].e_mem_type = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    memTab[MEM_REC_SPS].u4_mem_size = ((sizeof(dec_seq_params_t))
 * MAX_NUM_SEQ_PARAMS);

    memTab[MEM_REC_PPS].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_PPS].e_mem_type = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    memTab[MEM_REC_PPS].u4_mem_size = (sizeof(dec_pic_params_t))
 * MAX_NUM_PIC_PARAMS;

 {
        UWORD32 u4_mem_size;

        u4_mem_size = 0;
        u4_mem_size += (((sizeof(dec_err_status_t) + 127) >> 7) << 7);
        u4_mem_size += sizeof(sei);
        u4_mem_size += sizeof(dpb_commands_t);
        u4_mem_size += sizeof(dec_bit_stream_t);
        u4_mem_size += sizeof(dec_slice_params_t);
        u4_mem_size += MAX(sizeof(dec_seq_params_t), sizeof(dec_pic_params_t));

        memTab[MEM_REC_EXTRA_MEM].u4_mem_alignment = (128 * 8) / CHAR_BIT;
        memTab[MEM_REC_EXTRA_MEM].e_mem_type =
                        IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        memTab[MEM_REC_EXTRA_MEM].u4_mem_size = u4_mem_size;
 }

 {

        UWORD32 u4_mem_size;

        u4_mem_size = 0;
        u4_mem_size += ((TOTAL_LIST_ENTRIES + PAD_MAP_IDX_POC) * sizeof(void *));
        u4_mem_size = ALIGN64(u4_mem_size);
        u4_mem_size += (sizeof(bin_ctxt_model_t) * NUM_CABAC_CTXTS);
        u4_mem_size = ALIGN64(u4_mem_size);
        u4_mem_size += sizeof(ctxt_inc_mb_info_t);
        u4_mem_size = ALIGN64(u4_mem_size);
        u4_mem_size += sizeof(UWORD32) * (MAX_REF_BUFS * MAX_REF_BUFS);
        u4_mem_size = ALIGN64(u4_mem_size);

        u4_mem_size += MAX_REF_BUF_SIZE * 2;
        u4_mem_size = ALIGN64(u4_mem_size);
        u4_mem_size += ((sizeof(WORD16)) * PRED_BUFFER_WIDTH
 * PRED_BUFFER_HEIGHT * 2);
        u4_mem_size = ALIGN64(u4_mem_size);
        u4_mem_size += sizeof(UWORD8) * (MB_LUM_SIZE);
        u4_mem_size = ALIGN64(u4_mem_size);
        u4_mem_size += sizeof(parse_pmbarams_t) * luma_width_in_mbs;  
        u4_mem_size = ALIGN64(u4_mem_size);
        u4_mem_size += (sizeof(parse_part_params_t) * luma_width_in_mbs) << 4;  
        u4_mem_size = ALIGN64(u4_mem_size);

        u4_mem_size += 2 * MAX_REF_BUFS * sizeof(struct pic_buffer_t);
        u4_mem_size = ALIGN64(u4_mem_size);
        u4_mem_size += 2 * MAX_REF_BUFS * sizeof(struct pic_buffer_t);
        u4_mem_size = ALIGN64(u4_mem_size);
        u4_mem_size += (sizeof(UWORD32) * 3 * (MAX_REF_BUFS * MAX_REF_BUFS)) << 3;
        u4_mem_size = ALIGN64(u4_mem_size);

        u4_mem_size += sizeof(UWORD32) * 2 * 3
 * ((MAX_FRAMES << 1) * (MAX_FRAMES << 1));
        u4_mem_size = ALIGN64(u4_mem_size);

        memTab[MEM_REC_INTERNAL_SCRATCH].u4_mem_alignment =
 (128 * 8) / CHAR_BIT;
        memTab[MEM_REC_INTERNAL_SCRATCH].e_mem_type =
                        IV_EXTERNAL_CACHEABLE_SCRATCH_MEM;
        memTab[MEM_REC_INTERNAL_SCRATCH].u4_mem_size = u4_mem_size;
 }

 {

        UWORD32 u4_mem_used;
        UWORD32 u4_numRows = MB_SIZE << 1;
        UWORD32 u4_blk_wd = ((luma_width_in_mbs << 4) >> 1) + 8;

        u4_mem_used = 0;
        u4_mem_used += ((luma_width_in_mbs * sizeof(deblkmb_neighbour_t)) << 1);
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += (sizeof(neighbouradd_t) << 2);
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += ((sizeof(ctxt_inc_mb_info_t))
 * (((luma_width_in_mbs + 1) << 1) + 1));
        u4_mem_used = ALIGN64(u4_mem_used);

        u4_mem_used += (sizeof(mv_pred_t) * luma_width_in_mbs * 16);
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += (sizeof(mv_pred_t) * luma_width_in_mbs * 16);
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += (sizeof(mv_pred_t) * luma_width_in_mbs * 4
 * MV_SCRATCH_BUFS);
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += sizeof(UWORD8) * u4_numRows * u4_blk_wd;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += sizeof(UWORD8) * u4_numRows * u4_blk_wd;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_numRows = BLK8x8SIZE << 1;

        u4_blk_wd = ((luma_width_in_mbs << 3) >> 1) + 8;

        u4_mem_used += sizeof(UWORD8) * u4_numRows * u4_blk_wd;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += sizeof(UWORD8) * u4_numRows * u4_blk_wd;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += sizeof(UWORD8) * u4_numRows * u4_blk_wd;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += sizeof(UWORD8) * u4_numRows * u4_blk_wd;
        u4_mem_used += 32;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += sizeof(UWORD8) * (luma_width + 16) * 2;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += sizeof(UWORD8) * (luma_width + 16) * 2;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += sizeof(UWORD8) * (luma_width + 16) * 2;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += sizeof(mb_neigbour_params_t) * (luma_width_in_mbs + 1)
 * luma_height_in_mbs;
        u4_mem_used += luma_width;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += luma_width;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += luma_width;
        u4_mem_used = ALIGN64(u4_mem_used);

        u4_mem_used += ((MB_SIZE + 4) << 1) * PAD_LEN_Y_H;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += ((BLK8x8SIZE + 2) << 1) * PAD_LEN_UV_H;
        u4_mem_used = ALIGN64(u4_mem_used);
        u4_mem_used += ((BLK8x8SIZE + 2) << 1) * PAD_LEN_UV_H;
        u4_mem_used = ALIGN64(u4_mem_used);
        memTab[MEM_REC_INTERNAL_PERSIST].u4_mem_alignment =
 (128 * 8) / CHAR_BIT;
        memTab[MEM_REC_INTERNAL_PERSIST].e_mem_type =
                        IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        memTab[MEM_REC_INTERNAL_PERSIST].u4_mem_size = u4_mem_used;
 }

    memTab[MEM_REC_BITSBUF].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_BITSBUF].e_mem_type = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    memTab[MEM_REC_BITSBUF].u4_mem_size = MAX(256000, (luma_width * luma_height * 3 / 2));

 {

        UWORD32 u4_thread_struct_size = ithread_get_handle_size();

        memTab[MEM_REC_THREAD_HANDLE].u4_mem_alignment = (128 * 8) / CHAR_BIT;
        memTab[MEM_REC_THREAD_HANDLE].e_mem_type =
                        IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        memTab[MEM_REC_THREAD_HANDLE].u4_mem_size = u4_thread_struct_size * 2;

 }

    memTab[MEM_REC_PARSE_MAP].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_PARSE_MAP].e_mem_type = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    memTab[MEM_REC_PARSE_MAP].u4_mem_size = u4_total_num_mbs;

    memTab[MEM_REC_PROC_MAP].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_PROC_MAP].e_mem_type = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    memTab[MEM_REC_PROC_MAP].u4_mem_size = u4_total_num_mbs;

    memTab[MEM_REC_SLICE_NUM_MAP].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_SLICE_NUM_MAP].e_mem_type =
                    IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    memTab[MEM_REC_SLICE_NUM_MAP].u4_mem_size = u4_total_num_mbs
 * sizeof(UWORD16);

    memTab[MEM_REC_DPB_MGR].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_DPB_MGR].e_mem_type = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    memTab[MEM_REC_DPB_MGR].u4_mem_size = sizeof(dpb_manager_t);

    memTab[MEM_REC_BACKUP].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_BACKUP].e_mem_type = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    memTab[MEM_REC_BACKUP].u4_mem_size = sizeof(iv_mem_rec_t) * MEM_REC_CNT;

 {

        UWORD32 u4_mem_size;

        u4_mem_size = sizeof(disp_mgr_t);
        u4_mem_size += sizeof(buf_mgr_t) + ithread_get_mutex_lock_size();
        u4_mem_size += sizeof(struct pic_buffer_t) * (H264_MAX_REF_PICS * 2);

        memTab[MEM_REC_PIC_BUF_MGR].u4_mem_alignment = (128 * 8) / CHAR_BIT;
        memTab[MEM_REC_PIC_BUF_MGR].e_mem_type =
                        IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        memTab[MEM_REC_PIC_BUF_MGR].u4_mem_size = u4_mem_size;
 }

 {
        UWORD32 u4_mem_size;

        u4_mem_size  = sizeof(buf_mgr_t) + ithread_get_mutex_lock_size();
        u4_mem_size += sizeof(col_mv_buf_t) * (H264_MAX_REF_PICS * 2);
        u4_mem_size = ALIGN128(u4_mem_size);
        u4_mem_size += ((luma_width * luma_height) >> 4)
 * (MIN(max_dpb_size, num_ref_frames) + 1);
        memTab[MEM_REC_MV_BUF_MGR].u4_mem_alignment = (128 * 8) / CHAR_BIT;
        memTab[MEM_REC_MV_BUF_MGR].e_mem_type =
                        IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        memTab[MEM_REC_MV_BUF_MGR].u4_mem_size = u4_mem_size;
 }

    memTab[MEM_REC_PRED_INFO_PKD].u4_mem_alignment = (128 * 8) / CHAR_BIT;
    memTab[MEM_REC_PRED_INFO_PKD].e_mem_type =
                    IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;

 {
        UWORD32 u4_num_entries;
        u4_num_entries = u4_total_num_mbs;

 if(1 == num_ref_frames)
            u4_num_entries *= 16;
 else
            u4_num_entries *= 16 * 2;

        memTab[MEM_REC_PRED_INFO_PKD].u4_mem_size = sizeof(pred_info_pkd_t)
 * u4_num_entries;
 }

    ps_mem_q_op->s_ivd_fill_mem_rec_op_t.u4_num_mem_rec_filled = MEM_REC_CNT;


 return IV_SUCCESS;
}
