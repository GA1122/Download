IV_API_CALL_STATUS_T impeg2d_api_init(iv_obj_t *ps_dechdl,
 void *ps_ip,
 void *ps_op)
{
    UWORD32 i;

 void *pv;
    UWORD32 u4_size;

 dec_state_t *ps_dec_state;
 dec_state_multi_core_t *ps_dec_state_multi_core;
    UWORD32 u4_num_mem_rec;
 iv_mem_rec_t *ps_mem_rec ;
 iv_mem_rec_t *ps_frm_buf;
 iv_obj_t *ps_dec_handle;
    WORD32 i4_max_wd, i4_max_ht;

 impeg2d_init_ip_t *ps_dec_init_ip;
 impeg2d_init_op_t *ps_dec_init_op;
    WORD32 i4_num_threads;
    UWORD32 u4_share_disp_buf, u4_chroma_format;
    UWORD32 u4_deinterlace;

    ps_dec_init_ip = (impeg2d_init_ip_t *)ps_ip;
    ps_dec_init_op = (impeg2d_init_op_t *)ps_op;

    i4_max_wd = ALIGN16(ps_dec_init_ip->s_ivd_init_ip_t.u4_frm_max_wd);
    i4_max_ht = ALIGN16(ps_dec_init_ip->s_ivd_init_ip_t.u4_frm_max_ht);

 if(ps_dec_init_ip->s_ivd_init_ip_t.u4_size > offsetof(impeg2d_init_ip_t, u4_share_disp_buf))
 {
#ifndef LOGO_EN
        u4_share_disp_buf = ps_dec_init_ip->u4_share_disp_buf;
#else
        u4_share_disp_buf = 0;
#endif
 }
 else
 {
        u4_share_disp_buf = 0;
 }

    u4_chroma_format = ps_dec_init_ip->s_ivd_init_ip_t.e_output_format;

 if(ps_dec_init_ip->s_ivd_init_ip_t.u4_size > offsetof(impeg2d_init_ip_t, u4_deinterlace))
 {
        u4_deinterlace = ps_dec_init_ip->u4_deinterlace;
 }
 else
 {
        u4_deinterlace = 0;
 }

 if( (u4_chroma_format != IV_YUV_420P) &&
 (u4_chroma_format != IV_YUV_420SP_UV) &&
 (u4_chroma_format != IV_YUV_420SP_VU))
 {
        u4_share_disp_buf = 0;
 }

  
 if(u4_share_disp_buf)
 {
        u4_deinterlace = 0;
 }

    ps_mem_rec = ps_dec_init_ip->s_ivd_init_ip_t.pv_mem_rec_location;
    ps_mem_rec ++;


    ps_dec_init_op->s_ivd_init_op_t.u4_size = sizeof(impeg2d_init_op_t);


  
 for(i = 1; i < ps_dec_init_ip->s_ivd_init_ip_t.u4_num_mem_rec; i++)
 {
        memset(ps_mem_rec->pv_base,0,ps_mem_rec->u4_mem_size);
        ps_mem_rec++;
 }

  
    ps_mem_rec     = ps_dec_init_ip->s_ivd_init_ip_t.pv_mem_rec_location;


  
    ps_dec_handle  = ps_mem_rec->pv_base;
    u4_num_mem_rec = 1;
    ps_mem_rec++;





  
    ps_dec_state_multi_core = ps_mem_rec->pv_base;
    u4_num_mem_rec++;
    ps_mem_rec++;


 {
        ps_dec_handle->pv_codec_handle = (void *)ps_dec_state_multi_core;  

        ps_dechdl->pv_codec_handle = (void *)ps_dec_state_multi_core;
        ps_dechdl->pv_fxns = (void *)impeg2d_api_function;
 }


 for(i4_num_threads = 0; i4_num_threads < MAX_THREADS; i4_num_threads++)
 {
  
  
  
    ps_dec_state = ps_mem_rec->pv_base;

    ps_dec_state_multi_core->ps_dec_state[i4_num_threads] = ps_dec_state;

    ps_dec_state->ps_dec_state_multi_core = ps_dec_state_multi_core;

    ps_dec_state->i4_num_cores = 1;
    
     u4_num_mem_rec++;
     ps_mem_rec++;

  
     ps_dec_state->pv_codec_thread_handle = ps_mem_rec->pv_base;
     u4_num_mem_rec++;
     ps_mem_rec++;

  
  
  
    pv = ps_mem_rec->pv_base;

  

    ps_dec_state->s_mc_fw_buf.pu1_y = pv;
    pv = (void *)((UWORD8 *)pv + MB_LUMA_MEM_SIZE);

    u4_size = sizeof(UWORD8) * MB_LUMA_MEM_SIZE;
  

    ps_dec_state->s_mc_fw_buf.pu1_u = pv;
    pv = (void *)((UWORD8 *)pv + MB_CHROMA_MEM_SIZE);

    u4_size += sizeof(UWORD8) * MB_CHROMA_MEM_SIZE;

  

    ps_dec_state->s_mc_fw_buf.pu1_v = pv;
    pv = (void *)((UWORD8 *)pv + MB_CHROMA_MEM_SIZE);

    u4_size += sizeof(UWORD8) * MB_CHROMA_MEM_SIZE;

  

    ps_dec_state->s_mc_bk_buf.pu1_y = pv;
    pv = (void *)((UWORD8 *)pv + MB_LUMA_MEM_SIZE);

    u4_size += sizeof(UWORD8) * MB_LUMA_MEM_SIZE;

  

    ps_dec_state->s_mc_bk_buf.pu1_u = pv;
    pv = (void *)((UWORD8 *)pv + MB_CHROMA_MEM_SIZE);

    u4_size += sizeof(UWORD8) * MB_CHROMA_MEM_SIZE;

  

    ps_dec_state->s_mc_bk_buf.pu1_v = pv;
    pv = (void *)((UWORD8 *)pv + MB_CHROMA_MEM_SIZE);

    u4_size += sizeof(UWORD8) * MB_CHROMA_MEM_SIZE;

  

    ps_dec_state->s_mc_buf.pu1_y = pv;
    pv = (void *)((UWORD8 *)pv + MB_LUMA_MEM_SIZE);

    u4_size += sizeof(UWORD8) * MB_LUMA_MEM_SIZE;

  

    ps_dec_state->s_mc_buf.pu1_u = pv;
    pv = (void *)((UWORD8 *)pv + MB_CHROMA_MEM_SIZE);

    u4_size += sizeof(UWORD8) * MB_CHROMA_MEM_SIZE;

  

    ps_dec_state->s_mc_buf.pu1_v = pv;

    u4_size += sizeof(UWORD8) * MB_CHROMA_MEM_SIZE;

    u4_num_mem_rec++;
    ps_mem_rec++;



    ps_dec_state->pv_pic_buf_mg = 0;

  
  
  
    ps_dec_state->pv_stack_cntxt = ps_mem_rec->pv_base;
    u4_num_mem_rec++;
    ps_mem_rec++;

 }





  
  
  
    ps_dec_state = ps_dec_state_multi_core->ps_dec_state[0];

    ps_dec_state->pv_pic_buf_mg = ps_mem_rec->pv_base;
    ps_dec_state->pv_pic_buf_base = (UWORD8 *)ps_mem_rec->pv_base + sizeof(buf_mgr_t);

    u4_num_mem_rec++;
    ps_mem_rec++;



 for(i4_num_threads = 0; i4_num_threads < MAX_THREADS; i4_num_threads++)
 {

        ps_dec_state = ps_dec_state_multi_core->ps_dec_state[i4_num_threads];


  
  

        ps_dec_state->u2_header_done  = 0;  


 {
            UWORD32 u4_max_frm_width,u4_max_frm_height;

            u4_max_frm_width = ALIGN16(ps_dec_init_ip->s_ivd_init_ip_t.u4_frm_max_wd);
            u4_max_frm_height = ALIGN16(ps_dec_init_ip->s_ivd_init_ip_t.u4_frm_max_ht);

            ps_dec_state->u2_create_max_width   = u4_max_frm_width;
            ps_dec_state->u2_create_max_height  = u4_max_frm_height;

            ps_dec_state->i4_chromaFormat = ps_dec_init_ip->s_ivd_init_ip_t.e_output_format;
            ps_dec_state->u4_frm_buf_stride  = 0 ;
            ps_dec_state->u2_frame_width  = u4_max_frm_width;
            ps_dec_state->u2_picture_width  = u4_max_frm_width;
            ps_dec_state->u2_horizontal_size  = u4_max_frm_width;

            ps_dec_state->u2_frame_height = u4_max_frm_height;
            ps_dec_state->u2_vertical_size = u4_max_frm_height;
            ps_dec_state->u4_share_disp_buf = u4_share_disp_buf;
            ps_dec_state->u4_deinterlace = u4_deinterlace;
            ps_dec_state->ps_deint_pic = NULL;
 }
 }


    ps_dec_state = ps_dec_state_multi_core->ps_dec_state[0];

 if((ps_dec_state->i4_chromaFormat  == IV_YUV_422ILE)
 &&((ps_dec_state->u2_vertical_size & 0x1) != 0))
 {
        ps_dec_init_op->s_ivd_init_op_t.u4_error_code = IMPEG2D_INIT_CHROMA_FORMAT_HEIGHT_ERROR;
 return(IV_FAIL);


 }

  


 
    impeg2_disp_mgr_init(&ps_dec_state->s_disp_mgr);
    impeg2_buf_mgr_init((buf_mgr_t *)ps_dec_state->pv_pic_buf_mg);

  
  
  


  
 {
        ps_frm_buf = ps_mem_rec;
        memset(ps_frm_buf->pv_base, 128, ps_frm_buf->u4_mem_size);
        ps_frm_buf++;
 }

 if(0 == ps_dec_state->u4_share_disp_buf)
 {
 pic_buf_t *ps_pic_buf;
        ps_pic_buf = (pic_buf_t *)ps_dec_state->pv_pic_buf_base;
 for(i = 0; i < NUM_INT_FRAME_BUFFERS; i++)
 {
            UWORD8 *pu1_buf;
            pu1_buf = ps_mem_rec->pv_base;

            ps_pic_buf->pu1_y = pu1_buf;
            pu1_buf += i4_max_ht * i4_max_wd;

            ps_pic_buf->pu1_u = pu1_buf;
            pu1_buf += i4_max_ht * i4_max_wd >> 2;

            ps_pic_buf->pu1_v = pu1_buf;
            pu1_buf += i4_max_ht * i4_max_wd >> 2;

            ps_pic_buf->i4_buf_id = i;

            ps_pic_buf->u1_used_as_ref = 0;

            ps_pic_buf->u4_ts = 0;

            impeg2_buf_mgr_add(ps_dec_state->pv_pic_buf_mg, ps_pic_buf, i);
            ps_mem_rec++;
            ps_pic_buf++;
 }
        u4_num_mem_rec += NUM_INT_FRAME_BUFFERS;
 }
 else if (ps_dec_state->i4_chromaFormat  != IV_YUV_420P)
 {
 for(i = 0; i < NUM_INT_FRAME_BUFFERS; i++)
 {
            ps_dec_state->pu1_chroma_ref_buf[i] = ps_mem_rec->pv_base;
            ps_mem_rec++;
 }

        u4_num_mem_rec += NUM_INT_FRAME_BUFFERS;
 }
 else
 {
        ps_mem_rec+=NUM_INT_FRAME_BUFFERS;
        u4_num_mem_rec += NUM_INT_FRAME_BUFFERS;
 }


    ps_dec_state = ps_dec_state_multi_core->ps_dec_state[0];


    ps_dec_state->pv_jobq_buf = ps_mem_rec->pv_base;
    ps_dec_state->i4_jobq_buf_size = ps_mem_rec->u4_mem_size;
    ps_mem_rec++;

 if(u4_num_mem_rec > ps_dec_init_ip->s_ivd_init_ip_t.u4_num_mem_rec)
 {
        ps_dec_init_op->s_ivd_init_op_t.u4_error_code = IMPEG2D_INIT_NUM_MEM_REC_NOT_SUFFICIENT;
 return(IV_FAIL);

 }

    ps_dec_state->u1_flushfrm = 0;
    ps_dec_state->u1_flushcnt = 0;
    ps_dec_state->pv_jobq = impeg2_jobq_init(ps_dec_state->pv_jobq_buf, ps_dec_state->i4_jobq_buf_size);


    ps_dec_state->pv_deinterlacer_ctxt = ps_mem_rec->pv_base;
    ps_mem_rec++;

    ps_dec_state->pu1_deint_fmt_buf = ps_mem_rec->pv_base;
    ps_mem_rec++;


  
  
  
    ps_dec_state->pv_memTab     = (void *)ps_mem_rec->pv_base;
    memcpy(ps_mem_rec->pv_base,ps_dec_init_ip->s_ivd_init_ip_t.pv_mem_rec_location, ps_mem_rec->u4_mem_size);
  
    u4_num_mem_rec++;
    ps_mem_rec++;
    ps_dec_state->u4_num_mem_records = u4_num_mem_rec;


    ps_dec_state->u4_num_frames_decoded    = 0;
    ps_dec_state->aps_ref_pics[0] = NULL;
    ps_dec_state->aps_ref_pics[1] = NULL;

    ps_dec_init_op->s_ivd_init_op_t.u4_error_code = IV_SUCCESS;

    impeg2d_init_arch(ps_dec_state);

    impeg2d_init_function_ptr(ps_dec_state);

 return(IV_SUCCESS);
}
