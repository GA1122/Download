void impeg2d_fill_mem_rec(impeg2d_fill_mem_rec_ip_t *ps_ip,
 impeg2d_fill_mem_rec_op_t *ps_op)
{
    UWORD32 u4_i;

    UWORD8 u1_no_rec = 0;
    UWORD32 max_frm_width,max_frm_height,max_frm_size;
 iv_mem_rec_t *ps_mem_rec = ps_ip->s_ivd_fill_mem_rec_ip_t.pv_mem_rec_location;
    WORD32 i4_num_threads;
    WORD32 i4_share_disp_buf, i4_chroma_format;
    WORD32 i4_chroma_size;
    UWORD32 u4_deinterlace;
    UNUSED(u4_deinterlace);
    max_frm_width = ALIGN16(ps_ip->s_ivd_fill_mem_rec_ip_t.u4_max_frm_wd);
    max_frm_height = ALIGN16(ps_ip->s_ivd_fill_mem_rec_ip_t.u4_max_frm_ht);

    max_frm_size = (max_frm_width * max_frm_height * 3) >> 1; 

    i4_chroma_size = max_frm_width * max_frm_height / 4;

 if(ps_ip->s_ivd_fill_mem_rec_ip_t.u4_size > offsetof(impeg2d_fill_mem_rec_ip_t, u4_share_disp_buf))
 {
#ifndef LOGO_EN
        i4_share_disp_buf = ps_ip->u4_share_disp_buf;
#else
        i4_share_disp_buf = 0;
#endif
 }
 else
 {
        i4_share_disp_buf = 0;
 }
 if(ps_ip->s_ivd_fill_mem_rec_ip_t.u4_size > offsetof(impeg2d_fill_mem_rec_ip_t, e_output_format))
 {
        i4_chroma_format = ps_ip->e_output_format;
 }
 else
 {
        i4_chroma_format = -1;
 }

 if(ps_ip->s_ivd_fill_mem_rec_ip_t.u4_size > offsetof(impeg2d_fill_mem_rec_ip_t, u4_deinterlace))
 {
        u4_deinterlace = ps_ip->u4_deinterlace;
 }
 else
 {
        u4_deinterlace = 0;
 }


 if( (i4_chroma_format != IV_YUV_420P) &&
 (i4_chroma_format != IV_YUV_420SP_UV) &&
 (i4_chroma_format != IV_YUV_420SP_VU))
 {
        i4_share_disp_buf = 0;
 }

  
 if(i4_share_disp_buf)
 {
        u4_deinterlace = 0;
 }

  
  
  
  
    ps_mem_rec->u4_mem_alignment = 128  ;
    ps_mem_rec->e_mem_type      = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    ps_mem_rec->u4_mem_size     = sizeof(iv_obj_t);

    ps_mem_rec++;
    u1_no_rec++;

 {
  
  
  
  
        ps_mem_rec->u4_mem_alignment = 128  ;
        ps_mem_rec->e_mem_type      = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        ps_mem_rec->u4_mem_size     = sizeof(dec_state_multi_core_t);

        ps_mem_rec++;
        u1_no_rec++;
 }

 for(i4_num_threads = 0; i4_num_threads < MAX_THREADS; i4_num_threads++)
 {
  
  
  
  
        ps_mem_rec->u4_mem_alignment = 128  ;
        ps_mem_rec->e_mem_type      = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        ps_mem_rec->u4_mem_size     = sizeof(dec_state_t);

        ps_mem_rec++;
        u1_no_rec++;

  
        ps_mem_rec->u4_mem_alignment = 128  ;
        ps_mem_rec->e_mem_type      = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        ps_mem_rec->u4_mem_size     = ithread_get_handle_size();

        ps_mem_rec++;
        u1_no_rec++;

  
  
  
        ps_mem_rec->u4_mem_alignment = 128  ;
        ps_mem_rec->e_mem_type      = IV_EXTERNAL_CACHEABLE_SCRATCH_MEM;

  
        ps_mem_rec->u4_mem_size     = MB_LUMA_MEM_SIZE;

  
        ps_mem_rec->u4_mem_size    += MB_CHROMA_MEM_SIZE;

  
        ps_mem_rec->u4_mem_size    += MB_CHROMA_MEM_SIZE;

  
        ps_mem_rec->u4_mem_size    += MB_LUMA_MEM_SIZE;

  
        ps_mem_rec->u4_mem_size    += MB_CHROMA_MEM_SIZE;

  
        ps_mem_rec->u4_mem_size    += MB_CHROMA_MEM_SIZE;

  
        ps_mem_rec->u4_mem_size    += MB_LUMA_MEM_SIZE;

  
        ps_mem_rec->u4_mem_size    += MB_CHROMA_MEM_SIZE;

  
        ps_mem_rec->u4_mem_size    += MB_CHROMA_MEM_SIZE;

        ps_mem_rec++;
        u1_no_rec++;


  
  
  
  
        ps_mem_rec->u4_mem_alignment = 128  ;
        ps_mem_rec->e_mem_type      = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        ps_mem_rec->u4_mem_size     = 392;

        ps_mem_rec++;
        u1_no_rec++;
 }



 {
  
  
  
  
        ps_mem_rec->u4_mem_alignment = 128  ;
        ps_mem_rec->e_mem_type      = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
        ps_mem_rec->u4_mem_size     = sizeof(buf_mgr_t) + sizeof(pic_buf_t) * BUF_MGR_MAX_CNT;

        ps_mem_rec++;
        u1_no_rec++;
 }
  
  
  
 

 {
 for(u4_i = 0; u4_i < NUM_INT_FRAME_BUFFERS; u4_i++)
 {
  
            ps_mem_rec->u4_mem_alignment = 128  ;
            ps_mem_rec->e_mem_type      = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
 if(0 == i4_share_disp_buf)
                ps_mem_rec->u4_mem_size     = max_frm_size;
 else if(IV_YUV_420P != i4_chroma_format)
 {
  
                ps_mem_rec->u4_mem_size     = i4_chroma_size * 2;
 }
 else
                ps_mem_rec->u4_mem_size     = 64;
            ps_mem_rec++;
            u1_no_rec++;
 }
 }



 {
        WORD32 i4_job_queue_size;
        WORD32 i4_num_jobs;

  
        i4_num_jobs  = max_frm_height >> 4;

  
        i4_num_jobs  += max_frm_height >> 4;


        i4_job_queue_size = impeg2_jobq_ctxt_size();
        i4_job_queue_size += i4_num_jobs * sizeof(job_t);
        ps_mem_rec->u4_mem_size = i4_job_queue_size;
        ps_mem_rec->u4_mem_alignment = 128;
        ps_mem_rec->e_mem_type       = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;

        ps_mem_rec++;
        u1_no_rec++;

 }

    ps_mem_rec->u4_mem_alignment = 128;
    ps_mem_rec->e_mem_type       = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    ps_mem_rec->u4_mem_size      = impeg2d_deint_ctxt_size();
    ps_mem_rec++;
    u1_no_rec++;

    ps_mem_rec->u4_mem_alignment = 128;
    ps_mem_rec->e_mem_type       = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;

 if(IV_YUV_420P != i4_chroma_format)
        ps_mem_rec->u4_mem_size  = max_frm_size;
 else
        ps_mem_rec->u4_mem_size  = 64;

    ps_mem_rec++;
    u1_no_rec++;

    ps_mem_rec->u4_mem_alignment = 128;
    ps_mem_rec->e_mem_type       = IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM;
    ps_mem_rec->u4_mem_size      = sizeof(iv_mem_rec_t) * (NUM_MEM_RECORDS);
    ps_mem_rec++;
    u1_no_rec++;
    ps_op->s_ivd_fill_mem_rec_op_t.u4_num_mem_rec_filled = u1_no_rec;
    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code = 0;
}
