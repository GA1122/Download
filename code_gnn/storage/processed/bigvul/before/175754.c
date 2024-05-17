IV_API_CALL_STATUS_T impeg2d_api_check_struct_sanity(iv_obj_t *ps_handle,
 void *pv_api_ip,
 void *pv_api_op)
{
    WORD32  i4_cmd;
    UWORD32 *pu4_api_ip;
    UWORD32 *pu4_api_op;
    WORD32 i,j;

 if(NULL == pv_api_op)
 return(IV_FAIL);

 if(NULL == pv_api_ip)
 return(IV_FAIL);

    pu4_api_ip  = (UWORD32 *)pv_api_ip;
    pu4_api_op  = (UWORD32 *)pv_api_op;
    i4_cmd = (IVD_API_COMMAND_TYPE_T)*(pu4_api_ip + 1);

  
 switch(i4_cmd)
 {
 case IV_CMD_GET_NUM_MEM_REC:
 case IV_CMD_FILL_NUM_MEM_REC:
 break;
 case IV_CMD_INIT:
 if(ps_handle == NULL)
 {
 *(pu4_api_op + 1) |= 1 << IVD_UNSUPPORTEDPARAM;
 *(pu4_api_op + 1) |= IVD_HANDLE_NULL;
 return IV_FAIL;
 }

 if(ps_handle->u4_size != sizeof(iv_obj_t))
 {
 *(pu4_api_op + 1) |= 1 << IVD_UNSUPPORTEDPARAM;
 *(pu4_api_op + 1) |= IVD_HANDLE_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 break;
 case IVD_CMD_GET_DISPLAY_FRAME:
 case IVD_CMD_VIDEO_DECODE:
 case IV_CMD_RETRIEVE_MEMREC:
 case IVD_CMD_SET_DISPLAY_FRAME:
 case IVD_CMD_REL_DISPLAY_FRAME:
 case IVD_CMD_VIDEO_CTL:
 {
 if(ps_handle == NULL)
 {
 *(pu4_api_op + 1) |= 1 << IVD_UNSUPPORTEDPARAM;
 *(pu4_api_op + 1) |= IVD_HANDLE_NULL;
 return IV_FAIL;
 }

 if(ps_handle->u4_size != sizeof(iv_obj_t))
 {
 *(pu4_api_op + 1) |= 1 << IVD_UNSUPPORTEDPARAM;
 *(pu4_api_op + 1) |= IVD_HANDLE_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 if(ps_handle->pv_fxns != impeg2d_api_function)
 {
 *(pu4_api_op + 1) |= 1 << IVD_UNSUPPORTEDPARAM;
 *(pu4_api_op + 1) |= IVD_INVALID_HANDLE_NULL;
 return IV_FAIL;
 }

 if(ps_handle->pv_codec_handle == NULL)
 {
 *(pu4_api_op + 1) |= 1 << IVD_UNSUPPORTEDPARAM;
 *(pu4_api_op + 1) |= IVD_INVALID_HANDLE_NULL;
 return IV_FAIL;
 }
 }
 break;
 default:
 *(pu4_api_op + 1) |= 1 << IVD_UNSUPPORTEDPARAM;
 *(pu4_api_op + 1) |= IVD_INVALID_API_CMD;
 return IV_FAIL;
 }

 switch(i4_cmd)
 {
 case IV_CMD_GET_NUM_MEM_REC:
 {
 impeg2d_num_mem_rec_ip_t *ps_ip = (impeg2d_num_mem_rec_ip_t *)pv_api_ip;
 impeg2d_num_mem_rec_op_t *ps_op = (impeg2d_num_mem_rec_op_t *)pv_api_op;
                ps_op->s_ivd_num_mem_rec_op_t.u4_error_code = 0;

 if(ps_ip->s_ivd_num_mem_rec_ip_t.u4_size != sizeof(impeg2d_num_mem_rec_ip_t))
 {
                    ps_op->s_ivd_num_mem_rec_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_num_mem_rec_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 if(ps_op->s_ivd_num_mem_rec_op_t.u4_size != sizeof(impeg2d_num_mem_rec_op_t))
 {
                    ps_op->s_ivd_num_mem_rec_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_num_mem_rec_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }
 }
 break;
 case IV_CMD_FILL_NUM_MEM_REC:
 {
 impeg2d_fill_mem_rec_ip_t *ps_ip = (impeg2d_fill_mem_rec_ip_t *)pv_api_ip;
 impeg2d_fill_mem_rec_op_t *ps_op = (impeg2d_fill_mem_rec_op_t *)pv_api_op;
 iv_mem_rec_t *ps_mem_rec;

                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code = 0;

 if(ps_ip->s_ivd_fill_mem_rec_ip_t.u4_size != sizeof(impeg2d_fill_mem_rec_ip_t))
 {
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 if(ps_op->s_ivd_fill_mem_rec_op_t.u4_size != sizeof(impeg2d_fill_mem_rec_op_t))
 {
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 if(ps_ip->s_ivd_fill_mem_rec_ip_t.u4_max_frm_wd < MIN_WIDTH)
 {
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= IVD_REQUESTED_WIDTH_NOT_SUPPPORTED;
 return(IV_FAIL);
 }

 if(ps_ip->s_ivd_fill_mem_rec_ip_t.u4_max_frm_wd > MAX_WIDTH)
 {
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= IVD_REQUESTED_WIDTH_NOT_SUPPPORTED;
 return(IV_FAIL);
 }

 if(ps_ip->s_ivd_fill_mem_rec_ip_t.u4_max_frm_ht < MIN_HEIGHT)
 {
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= IVD_REQUESTED_HEIGHT_NOT_SUPPPORTED;
 return(IV_FAIL);
 }

 if(ps_ip->s_ivd_fill_mem_rec_ip_t.u4_max_frm_ht > MAX_HEIGHT)
 {
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= IVD_REQUESTED_HEIGHT_NOT_SUPPPORTED;
 return(IV_FAIL);
 }

 if(NULL == ps_ip->s_ivd_fill_mem_rec_ip_t.pv_mem_rec_location)
 {
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= IVD_NUM_REC_NOT_SUFFICIENT;
 return(IV_FAIL);
 }

  
                ps_mem_rec  = ps_ip->s_ivd_fill_mem_rec_ip_t.pv_mem_rec_location;
 for(i=0;i<NUM_MEM_RECORDS;i++)
 {
 if(ps_mem_rec[i].u4_size != sizeof(iv_mem_rec_t))
 {
                        ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= IVD_MEM_REC_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 }
 break;

 case IV_CMD_INIT:
 {
 impeg2d_init_ip_t *ps_ip = (impeg2d_init_ip_t *)pv_api_ip;
 impeg2d_init_op_t *ps_op = (impeg2d_init_op_t *)pv_api_op;
 iv_mem_rec_t *ps_mem_rec;
                UWORD32 u4_tot_num_mem_recs;

                ps_op->s_ivd_init_op_t.u4_error_code = 0;

 if(ps_ip->s_ivd_init_ip_t.u4_size != sizeof(impeg2d_init_ip_t))
 {
                    ps_op->s_ivd_init_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_init_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 if(ps_op->s_ivd_init_op_t.u4_size != sizeof(impeg2d_init_op_t))
 {
                    ps_op->s_ivd_init_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_init_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

                u4_tot_num_mem_recs = NUM_MEM_RECORDS;




 if(ps_ip->s_ivd_init_ip_t.u4_num_mem_rec > u4_tot_num_mem_recs)
 {
                    ps_op->s_ivd_init_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_NOT_SUFFICIENT;
 return(IV_FAIL);
 }

 if(ps_ip->s_ivd_init_ip_t.u4_frm_max_wd < MIN_WIDTH)
 {
                    ps_op->s_ivd_init_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_WIDTH_NOT_SUPPPORTED;
 return(IV_FAIL);
 }

 if(ps_ip->s_ivd_init_ip_t.u4_frm_max_wd > MAX_WIDTH)
 {
                    ps_op->s_ivd_init_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_WIDTH_NOT_SUPPPORTED;
 return(IV_FAIL);
 }

 if(ps_ip->s_ivd_init_ip_t.u4_frm_max_ht < MIN_HEIGHT)
 {
                    ps_op->s_ivd_init_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_HEIGHT_NOT_SUPPPORTED;
 return(IV_FAIL);
 }

 if(ps_ip->s_ivd_init_ip_t.u4_frm_max_ht > MAX_HEIGHT)
 {
                    ps_op->s_ivd_init_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_HEIGHT_NOT_SUPPPORTED;
 return(IV_FAIL);
 }

 if(NULL == ps_ip->s_ivd_init_ip_t.pv_mem_rec_location)
 {
                    ps_op->s_ivd_init_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_init_op_t.u4_error_code |= IVD_NUM_REC_NOT_SUFFICIENT;
 return(IV_FAIL);
 }

 if((ps_ip->s_ivd_init_ip_t.e_output_format != IV_YUV_420P) &&
 (ps_ip->s_ivd_init_ip_t.e_output_format != IV_YUV_422ILE)&&(ps_ip->s_ivd_init_ip_t.e_output_format != IV_YUV_420SP_UV)&&(ps_ip->s_ivd_init_ip_t.e_output_format != IV_YUV_420SP_VU))
 {
                    ps_op->s_ivd_init_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_COL_FMT_NOT_SUPPORTED;
 return(IV_FAIL);
 }

  
 if(ps_ip->s_ivd_init_ip_t.u4_num_mem_rec < NUM_MEM_RECORDS)
 {
                    ps_op->s_ivd_init_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_MEM_REC_NOT_SUFFICIENT;
 return IV_FAIL;
 }

                ps_mem_rec  = ps_ip->s_ivd_init_ip_t.pv_mem_rec_location;
  
  
  
 for(i=0;i < (WORD32)ps_ip->s_ivd_init_ip_t.u4_num_mem_rec ; i++)
 {
 if(ps_mem_rec[i].u4_size != sizeof(iv_mem_rec_t))
 {
                        ps_op->s_ivd_init_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_init_op_t.u4_error_code |= IVD_MEM_REC_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }

  
 {
                    UWORD8 *pau1_start[NUM_MEM_RECORDS];
                    UWORD8 *pau1_end[NUM_MEM_RECORDS];


                    pau1_start[0] = (UWORD8 *)(ps_mem_rec[0].pv_base);
                    pau1_end[0] = (UWORD8 *)(ps_mem_rec[0].pv_base) + ps_mem_rec[0].u4_mem_size - 1;
 for(i = 1; i < (WORD32)ps_ip->s_ivd_init_ip_t.u4_num_mem_rec; i++)
 {
  
                        pau1_start[i] = (UWORD8 *)(ps_mem_rec[i].pv_base);
                        pau1_end[i] = (UWORD8 *)(ps_mem_rec[i].pv_base) + ps_mem_rec[i].u4_mem_size - 1;

 for(j = 0; j < i; j++)
 {
 if((pau1_start[i] >= pau1_start[j]) && (pau1_start[i] <= pau1_end[j]))
 {
                                ps_op->s_ivd_init_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_MEM_REC_OVERLAP_ERR;
 return IV_FAIL;
 }

 if((pau1_end[i] >= pau1_start[j]) && (pau1_end[i] <= pau1_end[j]))
 {
                                ps_op->s_ivd_init_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_MEM_REC_OVERLAP_ERR;
 return IV_FAIL;
 }

 if((pau1_start[i] < pau1_start[j]) && (pau1_end[i] > pau1_end[j]))
 {
                                ps_op->s_ivd_init_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_MEM_REC_OVERLAP_ERR;
 return IV_FAIL;
 }
 }
 }
 }




 {
 iv_mem_rec_t    as_mem_rec_ittiam_api[NUM_MEM_RECORDS];

 impeg2d_fill_mem_rec_ip_t s_fill_mem_rec_ip;
 impeg2d_fill_mem_rec_op_t s_fill_mem_rec_op;
                    IV_API_CALL_STATUS_T e_status;
                    WORD32 i4_num_memrec;
 {

 iv_num_mem_rec_ip_t s_no_of_mem_rec_query_ip;
 iv_num_mem_rec_op_t s_no_of_mem_rec_query_op;


                        s_no_of_mem_rec_query_ip.u4_size = sizeof(iv_num_mem_rec_ip_t);
                        s_no_of_mem_rec_query_op.u4_size = sizeof(iv_num_mem_rec_op_t);

                        s_no_of_mem_rec_query_ip.e_cmd   = IV_CMD_GET_NUM_MEM_REC;
                        impeg2d_api_function(NULL,
 (void *)&s_no_of_mem_rec_query_ip,
 (void *)&s_no_of_mem_rec_query_op);

                        i4_num_memrec  = s_no_of_mem_rec_query_op.u4_num_mem_rec;



 }


  
 for(i = 0; i < i4_num_memrec; i++)
 {
                        as_mem_rec_ittiam_api[i].u4_size = sizeof(iv_mem_rec_t);
 }

                    s_fill_mem_rec_ip.s_ivd_fill_mem_rec_ip_t.u4_size                   = sizeof(impeg2d_fill_mem_rec_ip_t);
                    s_fill_mem_rec_ip.s_ivd_fill_mem_rec_ip_t.e_cmd                     = IV_CMD_FILL_NUM_MEM_REC;
                    s_fill_mem_rec_ip.s_ivd_fill_mem_rec_ip_t.u4_max_frm_wd             = ps_ip->s_ivd_init_ip_t.u4_frm_max_wd;
                    s_fill_mem_rec_ip.s_ivd_fill_mem_rec_ip_t.u4_max_frm_ht             = ps_ip->s_ivd_init_ip_t.u4_frm_max_ht;
                    s_fill_mem_rec_ip.s_ivd_fill_mem_rec_ip_t.pv_mem_rec_location       = as_mem_rec_ittiam_api;
                    s_fill_mem_rec_ip.u4_share_disp_buf                                 = ps_ip->u4_share_disp_buf;
                    s_fill_mem_rec_ip.e_output_format                                   = ps_ip->s_ivd_init_ip_t.e_output_format;
                    s_fill_mem_rec_op.s_ivd_fill_mem_rec_op_t.u4_size                   = sizeof(impeg2d_fill_mem_rec_op_t);


                    e_status = impeg2d_api_function(NULL,
 (void *)&s_fill_mem_rec_ip,
 (void *)&s_fill_mem_rec_op);
 if(IV_FAIL == e_status)
 {
                        ps_op->s_ivd_init_op_t.u4_error_code = s_fill_mem_rec_op.s_ivd_fill_mem_rec_op_t.u4_error_code;
 return(IV_FAIL);
 }



 for(i = 0; i < i4_num_memrec; i ++)
 {
 if(ps_mem_rec[i].pv_base == NULL)
 {
                            ps_op->s_ivd_init_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_MEM_REC_BASE_NULL;
 return IV_FAIL;
 }
#ifdef CHECK_ALIGN

 if((UWORD32)(ps_mem_rec[i].pv_base) & (ps_mem_rec[i].u4_mem_alignment - 1))
 {
                            ps_op->s_ivd_init_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_MEM_REC_ALIGNMENT_ERR;
 return IV_FAIL;
 }
#endif  
 if(ps_mem_rec[i].u4_mem_alignment != as_mem_rec_ittiam_api[i].u4_mem_alignment)
 {
                            ps_op->s_ivd_init_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_MEM_REC_ALIGNMENT_ERR;
 return IV_FAIL;
 }

 if(ps_mem_rec[i].u4_mem_size < as_mem_rec_ittiam_api[i].u4_mem_size)
 {
                            ps_op->s_ivd_init_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_MEM_REC_INSUFFICIENT_SIZE;
 return IV_FAIL;
 }

 if(ps_mem_rec[i].e_mem_type != as_mem_rec_ittiam_api[i].e_mem_type)
 {
 if (IV_EXTERNAL_CACHEABLE_SCRATCH_MEM == as_mem_rec_ittiam_api[i].e_mem_type)
 {
 if (IV_EXTERNAL_CACHEABLE_PERSISTENT_MEM == ps_mem_rec[i].e_mem_type)
 {
 continue;
 }
 }
                            ps_op->s_ivd_init_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->s_ivd_init_op_t.u4_error_code |= IVD_INIT_DEC_MEM_REC_INCORRECT_TYPE;
 return IV_FAIL;
 }
 }
 }


 }
 break;

 case IVD_CMD_GET_DISPLAY_FRAME:
 {
 impeg2d_get_display_frame_ip_t *ps_ip = (impeg2d_get_display_frame_ip_t *)pv_api_ip;
 impeg2d_get_display_frame_op_t *ps_op = (impeg2d_get_display_frame_op_t *)pv_api_op;

                ps_op->s_ivd_get_display_frame_op_t.u4_error_code = 0;

 if(ps_ip->s_ivd_get_display_frame_ip_t.u4_size != sizeof(impeg2d_get_display_frame_ip_t))
 {
                    ps_op->s_ivd_get_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_get_display_frame_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 if(ps_op->s_ivd_get_display_frame_op_t.u4_size != sizeof(impeg2d_get_display_frame_op_t))
 {
                    ps_op->s_ivd_get_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_get_display_frame_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 if(ps_ip->s_ivd_get_display_frame_ip_t.s_out_buffer.u4_num_bufs == 0)
 {
                    ps_op->s_ivd_get_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_get_display_frame_op_t.u4_error_code |= IVD_DISP_FRM_ZERO_OP_BUFS;
 return IV_FAIL;
 }

 for(i = 0; i< (WORD32)ps_ip->s_ivd_get_display_frame_ip_t.s_out_buffer.u4_num_bufs;i++)
 {
 if(ps_ip->s_ivd_get_display_frame_ip_t.s_out_buffer.pu1_bufs[i] == NULL)
 {
                        ps_op->s_ivd_get_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_get_display_frame_op_t.u4_error_code |= IVD_DISP_FRM_OP_BUF_NULL;
 return IV_FAIL;
 }

 if(ps_ip->s_ivd_get_display_frame_ip_t.s_out_buffer.u4_min_out_buf_size[i] == 0)
 {
                        ps_op->s_ivd_get_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_get_display_frame_op_t.u4_error_code |= IVD_DISP_FRM_ZERO_OP_BUF_SIZE;
 return IV_FAIL;
 }
  
 }
 }
 break;
 case IVD_CMD_REL_DISPLAY_FRAME:
 {
 impeg2d_rel_display_frame_ip_t *ps_ip = (impeg2d_rel_display_frame_ip_t *)pv_api_ip;
 impeg2d_rel_display_frame_op_t *ps_op = (impeg2d_rel_display_frame_op_t *)pv_api_op;

                ps_op->s_ivd_rel_display_frame_op_t.u4_error_code = 0;

 if ((ps_ip->s_ivd_rel_display_frame_ip_t.u4_size != sizeof(impeg2d_rel_display_frame_ip_t))
 && (ps_ip->s_ivd_rel_display_frame_ip_t.u4_size != sizeof(ivd_rel_display_frame_ip_t)))
 {
                    ps_op->s_ivd_rel_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_rel_display_frame_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 if((ps_op->s_ivd_rel_display_frame_op_t.u4_size != sizeof(impeg2d_rel_display_frame_op_t)) &&
 (ps_op->s_ivd_rel_display_frame_op_t.u4_size != sizeof(ivd_rel_display_frame_op_t)))
 {
                    ps_op->s_ivd_rel_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_rel_display_frame_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 }
 break;


 case IVD_CMD_SET_DISPLAY_FRAME:
 {
 impeg2d_set_display_frame_ip_t *ps_ip = (impeg2d_set_display_frame_ip_t *)pv_api_ip;
 impeg2d_set_display_frame_op_t *ps_op = (impeg2d_set_display_frame_op_t *)pv_api_op;
                UWORD32 j, i;

                ps_op->s_ivd_set_display_frame_op_t.u4_error_code = 0;

 if ((ps_ip->s_ivd_set_display_frame_ip_t.u4_size != sizeof(impeg2d_set_display_frame_ip_t))
 && (ps_ip->s_ivd_set_display_frame_ip_t.u4_size != sizeof(ivd_set_display_frame_ip_t)))
 {
                    ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 if((ps_op->s_ivd_set_display_frame_op_t.u4_size != sizeof(impeg2d_set_display_frame_op_t)) &&
 (ps_op->s_ivd_set_display_frame_op_t.u4_size != sizeof(ivd_set_display_frame_op_t)))
 {
                    ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 if(ps_ip->s_ivd_set_display_frame_ip_t.num_disp_bufs == 0)
 {
                    ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= IVD_DISP_FRM_ZERO_OP_BUFS;
 return IV_FAIL;
 }

 for(j = 0; j < ps_ip->s_ivd_set_display_frame_ip_t.num_disp_bufs; j++)
 {
 if(ps_ip->s_ivd_set_display_frame_ip_t.s_disp_buffer[j].u4_num_bufs == 0)
 {
                        ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= IVD_DISP_FRM_ZERO_OP_BUFS;
 return IV_FAIL;
 }

 for(i=0;i< ps_ip->s_ivd_set_display_frame_ip_t.s_disp_buffer[j].u4_num_bufs;i++)
 {
 if(ps_ip->s_ivd_set_display_frame_ip_t.s_disp_buffer[j].pu1_bufs[i] == NULL)
 {
                            ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= IVD_DISP_FRM_OP_BUF_NULL;
 return IV_FAIL;
 }

 if(ps_ip->s_ivd_set_display_frame_ip_t.s_disp_buffer[j].u4_min_out_buf_size[i] == 0)
 {
                            ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= IVD_DISP_FRM_ZERO_OP_BUF_SIZE;
 return IV_FAIL;
 }
 }
 }
 }
 break;

 case IVD_CMD_VIDEO_DECODE:
 {
 impeg2d_video_decode_ip_t *ps_ip = (impeg2d_video_decode_ip_t *)pv_api_ip;
 impeg2d_video_decode_op_t *ps_op = (impeg2d_video_decode_op_t *)pv_api_op;

                ps_op->s_ivd_video_decode_op_t.u4_error_code = 0;

 if(ps_ip->s_ivd_video_decode_ip_t.u4_size != sizeof(impeg2d_video_decode_ip_t))
 {
                    ps_op->s_ivd_video_decode_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_video_decode_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 if(ps_op->s_ivd_video_decode_op_t.u4_size != sizeof(impeg2d_video_decode_op_t))
 {
                    ps_op->s_ivd_video_decode_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_video_decode_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 }
 break;

 case IV_CMD_RETRIEVE_MEMREC:
 {
 impeg2d_retrieve_mem_rec_ip_t *ps_ip = (impeg2d_retrieve_mem_rec_ip_t *)pv_api_ip;
 impeg2d_retrieve_mem_rec_op_t *ps_op = (impeg2d_retrieve_mem_rec_op_t *)pv_api_op;
 iv_mem_rec_t *ps_mem_rec;

                ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code = 0;

 if(ps_ip->s_ivd_retrieve_mem_rec_ip_t.u4_size != sizeof(impeg2d_retrieve_mem_rec_ip_t))
 {
                    ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

 if(ps_op->s_ivd_retrieve_mem_rec_op_t.u4_size != sizeof(impeg2d_retrieve_mem_rec_op_t))
 {
                    ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return(IV_FAIL);
 }

                ps_mem_rec  = ps_ip->s_ivd_retrieve_mem_rec_ip_t.pv_mem_rec_location;
  
 for(i=0;i < NUM_MEM_RECORDS ; i++)
 {
 if(ps_mem_rec[i].u4_size != sizeof(iv_mem_rec_t))
 {
                        ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code |= IVD_MEM_REC_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 }
 break;

 case IVD_CMD_VIDEO_CTL:
 {
                UWORD32 *pu4_ptr_cmd;
                UWORD32 u4_sub_command;

                pu4_ptr_cmd = (UWORD32 *)pv_api_ip;
                pu4_ptr_cmd += 2;
                u4_sub_command = *pu4_ptr_cmd;

 switch(u4_sub_command)
 {
 case IVD_CMD_CTL_SETPARAMS:
 {
 impeg2d_ctl_set_config_ip_t *ps_ip;
 impeg2d_ctl_set_config_op_t *ps_op;
                            ps_ip = (impeg2d_ctl_set_config_ip_t *)pv_api_ip;
                            ps_op = (impeg2d_ctl_set_config_op_t *)pv_api_op;

                            ps_op->s_ivd_ctl_set_config_op_t.u4_error_code = 0;

 if(ps_ip->s_ivd_ctl_set_config_ip_t.u4_size != sizeof(impeg2d_ctl_set_config_ip_t))
 {
                                ps_op->s_ivd_ctl_set_config_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_ctl_set_config_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 case IVD_CMD_CTL_SETDEFAULT:
 {
 impeg2d_ctl_set_config_op_t *ps_op;
                            ps_op = (impeg2d_ctl_set_config_op_t *)pv_api_op;
                            ps_op->s_ivd_ctl_set_config_op_t.u4_error_code   = 0;

 if(ps_op->s_ivd_ctl_set_config_op_t.u4_size != sizeof(impeg2d_ctl_set_config_op_t))
 {
                                ps_op->s_ivd_ctl_set_config_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_ctl_set_config_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 break;

 case IVD_CMD_CTL_GETPARAMS:
 {
 impeg2d_ctl_getstatus_ip_t *ps_ip;
 impeg2d_ctl_getstatus_op_t *ps_op;

                            ps_ip = (impeg2d_ctl_getstatus_ip_t *)pv_api_ip;
                            ps_op = (impeg2d_ctl_getstatus_op_t *)pv_api_op;

                            ps_op->s_ivd_ctl_getstatus_op_t.u4_error_code   = 0;

 if(ps_ip->s_ivd_ctl_getstatus_ip_t.u4_size != sizeof(impeg2d_ctl_getstatus_ip_t))
 {
                                ps_op->s_ivd_ctl_getstatus_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_ctl_getstatus_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 if(ps_op->s_ivd_ctl_getstatus_op_t.u4_size != sizeof(impeg2d_ctl_getstatus_op_t))
 {
                                ps_op->s_ivd_ctl_getstatus_op_t.u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_ctl_getstatus_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 break;

 case IVD_CMD_CTL_GETBUFINFO:
 {
 impeg2d_ctl_getbufinfo_ip_t *ps_ip;
 impeg2d_ctl_getbufinfo_op_t *ps_op;
                            ps_ip = (impeg2d_ctl_getbufinfo_ip_t *)pv_api_ip;
                            ps_op = (impeg2d_ctl_getbufinfo_op_t *)pv_api_op;

                            ps_op->s_ivd_ctl_getbufinfo_op_t.u4_error_code  = 0;

 if(ps_ip->s_ivd_ctl_getbufinfo_ip_t.u4_size != sizeof(impeg2d_ctl_getbufinfo_ip_t))
 {
                                ps_op->s_ivd_ctl_getbufinfo_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_ctl_getbufinfo_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 if(ps_op->s_ivd_ctl_getbufinfo_op_t.u4_size != sizeof(impeg2d_ctl_getbufinfo_op_t))
 {
                                ps_op->s_ivd_ctl_getbufinfo_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_ctl_getbufinfo_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 break;

 case IVD_CMD_CTL_GETVERSION:
 {
 impeg2d_ctl_getversioninfo_ip_t *ps_ip;
 impeg2d_ctl_getversioninfo_op_t *ps_op;
                            ps_ip = (impeg2d_ctl_getversioninfo_ip_t *)pv_api_ip;
                            ps_op = (impeg2d_ctl_getversioninfo_op_t *)pv_api_op;

                            ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code  = 0;

 if(ps_ip->s_ivd_ctl_getversioninfo_ip_t.u4_size != sizeof(impeg2d_ctl_getversioninfo_ip_t))
 {
                                ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 if(ps_op->s_ivd_ctl_getversioninfo_op_t.u4_size != sizeof(impeg2d_ctl_getversioninfo_op_t))
 {
                                ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 break;

 case IVD_CMD_CTL_FLUSH:
 {
 impeg2d_ctl_flush_ip_t *ps_ip;
 impeg2d_ctl_flush_op_t *ps_op;
                            ps_ip = (impeg2d_ctl_flush_ip_t *)pv_api_ip;
                            ps_op = (impeg2d_ctl_flush_op_t *)pv_api_op;

                            ps_op->s_ivd_ctl_flush_op_t.u4_error_code = 0;

 if(ps_ip->s_ivd_ctl_flush_ip_t.u4_size != sizeof(impeg2d_ctl_flush_ip_t))
 {
                                ps_op->s_ivd_ctl_flush_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_ctl_flush_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 if(ps_op->s_ivd_ctl_flush_op_t.u4_size != sizeof(impeg2d_ctl_flush_op_t))
 {
                                ps_op->s_ivd_ctl_flush_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_ctl_flush_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 break;

 case IVD_CMD_CTL_RESET:
 {
 impeg2d_ctl_reset_ip_t *ps_ip;
 impeg2d_ctl_reset_op_t *ps_op;
                            ps_ip = (impeg2d_ctl_reset_ip_t *)pv_api_ip;
                            ps_op = (impeg2d_ctl_reset_op_t *)pv_api_op;

                            ps_op->s_ivd_ctl_reset_op_t.u4_error_code    = 0;

 if(ps_ip->s_ivd_ctl_reset_ip_t.u4_size != sizeof(impeg2d_ctl_reset_ip_t))
 {
                                ps_op->s_ivd_ctl_reset_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_ctl_reset_op_t.u4_error_code |= IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 if(ps_op->s_ivd_ctl_reset_op_t.u4_size != sizeof(impeg2d_ctl_reset_op_t))
 {
                                ps_op->s_ivd_ctl_reset_op_t.u4_error_code  |= 1 << IVD_UNSUPPORTEDPARAM;
                                ps_op->s_ivd_ctl_reset_op_t.u4_error_code |= IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 break;

 case IMPEG2D_CMD_CTL_GET_BUFFER_DIMENSIONS:
 {
 impeg2d_ctl_get_frame_dimensions_ip_t *ps_ip;
 impeg2d_ctl_get_frame_dimensions_op_t *ps_op;

                        ps_ip =
 (impeg2d_ctl_get_frame_dimensions_ip_t *)pv_api_ip;
                        ps_op =
 (impeg2d_ctl_get_frame_dimensions_op_t *)pv_api_op;

 if(ps_ip->u4_size
 != sizeof(impeg2d_ctl_get_frame_dimensions_ip_t))
 {
                            ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->u4_error_code |=
                                            IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 if(ps_op->u4_size
 != sizeof(impeg2d_ctl_get_frame_dimensions_op_t))
 {
                            ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->u4_error_code |=
                                            IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 break;
 }
 case IMPEG2D_CMD_CTL_GET_SEQ_INFO:
 {
 impeg2d_ctl_get_seq_info_ip_t *ps_ip;
 impeg2d_ctl_get_seq_info_op_t *ps_op;

                        ps_ip =
 (impeg2d_ctl_get_seq_info_ip_t *)pv_api_ip;
                        ps_op =
 (impeg2d_ctl_get_seq_info_op_t *)pv_api_op;

 if(ps_ip->u4_size
 != sizeof(impeg2d_ctl_get_seq_info_ip_t))
 {
                            ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->u4_error_code |=
                                            IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 if(ps_op->u4_size
 != sizeof(impeg2d_ctl_get_seq_info_op_t))
 {
                            ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->u4_error_code |=
                                            IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 break;
 }
 case IMPEG2D_CMD_CTL_SET_NUM_CORES:
 {
 impeg2d_ctl_set_num_cores_ip_t *ps_ip;
 impeg2d_ctl_set_num_cores_op_t *ps_op;

                        ps_ip = (impeg2d_ctl_set_num_cores_ip_t *)pv_api_ip;
                        ps_op = (impeg2d_ctl_set_num_cores_op_t *)pv_api_op;

 if(ps_ip->u4_size
 != sizeof(impeg2d_ctl_set_num_cores_ip_t))
 {
                            ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->u4_error_code |=
                                            IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 if(ps_op->u4_size
 != sizeof(impeg2d_ctl_set_num_cores_op_t))
 {
                            ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->u4_error_code |=
                                            IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

#ifdef MULTICORE
 if((ps_ip->u4_num_cores < 1) || (ps_ip->u4_num_cores > MAX_THREADS))
#else
 if(ps_ip->u4_num_cores != 1)
#endif
 {
                            ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
 return IV_FAIL;
 }
 break;
 }
 case IMPEG2D_CMD_CTL_SET_PROCESSOR:
 {
 impeg2d_ctl_set_processor_ip_t *ps_ip;
 impeg2d_ctl_set_processor_op_t *ps_op;

                        ps_ip = (impeg2d_ctl_set_processor_ip_t *)pv_api_ip;
                        ps_op = (impeg2d_ctl_set_processor_op_t *)pv_api_op;

 if(ps_ip->u4_size
 != sizeof(impeg2d_ctl_set_processor_ip_t))
 {
                            ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->u4_error_code |=
                                            IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 if(ps_op->u4_size
 != sizeof(impeg2d_ctl_set_processor_op_t))
 {
                            ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                            ps_op->u4_error_code |=
                                            IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 break;
 }
 default:
 break;

 }
 }
 break;

 default:
 { *(pu4_api_op + 1) |= 1 << IVD_UNSUPPORTEDPARAM;
 *(pu4_api_op + 1) |= IVD_UNSUPPORTED_API_CMD;
 return IV_FAIL;
 }


 }

 return IV_SUCCESS;
}
