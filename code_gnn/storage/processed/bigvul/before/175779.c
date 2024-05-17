static IV_API_CALL_STATUS_T api_check_struct_sanity(iv_obj_t *ps_handle,
 void *pv_api_ip,
 void *pv_api_op)
{
    IVD_API_COMMAND_TYPE_T e_cmd;
    UWORD32 *pu4_api_ip;
    UWORD32 *pu4_api_op;
    UWORD32 i, j;

 if(NULL == pv_api_op)
 return (IV_FAIL);

 if(NULL == pv_api_ip)
 return (IV_FAIL);

    pu4_api_ip = (UWORD32 *)pv_api_ip;
    pu4_api_op = (UWORD32 *)pv_api_op;
    e_cmd = *(pu4_api_ip + 1);

  
 switch((WORD32)e_cmd)
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
                H264_DEC_DEBUG_PRINT(
 "Sizes do not match. Expected: %d, Got: %d",
 sizeof(iv_obj_t), ps_handle->u4_size);
 return IV_FAIL;
 }
 break;
 case IVD_CMD_REL_DISPLAY_FRAME:
 case IVD_CMD_SET_DISPLAY_FRAME:
 case IVD_CMD_GET_DISPLAY_FRAME:
 case IVD_CMD_VIDEO_DECODE:
 case IV_CMD_RETRIEVE_MEMREC:
 case IVD_CMD_VIDEO_CTL:
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

 if(ps_handle->pv_fxns != ih264d_api_function)
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
 break;
 default:
 *(pu4_api_op + 1) |= 1 << IVD_UNSUPPORTEDPARAM;
 *(pu4_api_op + 1) |= IVD_INVALID_API_CMD;
 return IV_FAIL;
 }

 switch((WORD32)e_cmd)
 {
 case IV_CMD_GET_NUM_MEM_REC:
 {
 ih264d_num_mem_rec_ip_t *ps_ip =
 (ih264d_num_mem_rec_ip_t *)pv_api_ip;
 ih264d_num_mem_rec_op_t *ps_op =
 (ih264d_num_mem_rec_op_t *)pv_api_op;
            ps_op->s_ivd_num_mem_rec_op_t.u4_error_code = 0;

 if(ps_ip->s_ivd_num_mem_rec_ip_t.u4_size
 != sizeof(ih264d_num_mem_rec_ip_t))
 {
                ps_op->s_ivd_num_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_num_mem_rec_op_t.u4_error_code |=
                                IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

 if(ps_op->s_ivd_num_mem_rec_op_t.u4_size
 != sizeof(ih264d_num_mem_rec_op_t))
 {
                ps_op->s_ivd_num_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_num_mem_rec_op_t.u4_error_code |=
                                IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }
 }
 break;
 case IV_CMD_FILL_NUM_MEM_REC:
 {
 ih264d_fill_mem_rec_ip_t *ps_ip =
 (ih264d_fill_mem_rec_ip_t *)pv_api_ip;
 ih264d_fill_mem_rec_op_t *ps_op =
 (ih264d_fill_mem_rec_op_t *)pv_api_op;
 iv_mem_rec_t *ps_mem_rec;
            WORD32 max_wd = ps_ip->s_ivd_fill_mem_rec_ip_t.u4_max_frm_wd;
            WORD32 max_ht = ps_ip->s_ivd_fill_mem_rec_ip_t.u4_max_frm_ht;

            max_wd = ALIGN16(max_wd);
            max_ht = ALIGN32(max_ht);

            ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code = 0;

 if((ps_ip->s_ivd_fill_mem_rec_ip_t.u4_size
 > sizeof(ih264d_fill_mem_rec_ip_t))
 || (ps_ip->s_ivd_fill_mem_rec_ip_t.u4_size
 < sizeof(iv_fill_mem_rec_ip_t)))
 {
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |=
                                IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

 if((ps_op->s_ivd_fill_mem_rec_op_t.u4_size
 != sizeof(ih264d_fill_mem_rec_op_t))
 && (ps_op->s_ivd_fill_mem_rec_op_t.u4_size
 != sizeof(iv_fill_mem_rec_op_t)))
 {
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |=
                                IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

 if(max_wd < H264_MIN_FRAME_WIDTH)
 {
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |=
                                IVD_REQUESTED_WIDTH_NOT_SUPPPORTED;
 return (IV_FAIL);
 }

 if(max_wd > H264_MAX_FRAME_WIDTH)
 {
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |=
                                IVD_REQUESTED_WIDTH_NOT_SUPPPORTED;
 return (IV_FAIL);
 }

 if(max_ht < H264_MIN_FRAME_HEIGHT)
 {
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |=
                                IVD_REQUESTED_HEIGHT_NOT_SUPPPORTED;
 return (IV_FAIL);
 }

 if((max_ht * max_wd)
 > (H264_MAX_FRAME_HEIGHT * H264_MAX_FRAME_WIDTH))

 {
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |=
                                IVD_REQUESTED_HEIGHT_NOT_SUPPPORTED;
 return (IV_FAIL);
 }

 if(NULL == ps_ip->s_ivd_fill_mem_rec_ip_t.pv_mem_rec_location)
 {
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |=
                                IVD_NUM_REC_NOT_SUFFICIENT;
 return (IV_FAIL);
 }

  
            ps_mem_rec = ps_ip->s_ivd_fill_mem_rec_ip_t.pv_mem_rec_location;
 for(i = 0; i < MEM_REC_CNT; i++)
 {
 if(ps_mem_rec[i].u4_size != sizeof(iv_mem_rec_t))
 {
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_fill_mem_rec_op_t.u4_error_code |=
                                    IVD_MEM_REC_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 }
 break;

 case IV_CMD_INIT:
 {
 ih264d_init_ip_t *ps_ip = (ih264d_init_ip_t *)pv_api_ip;
 ih264d_init_op_t *ps_op = (ih264d_init_op_t *)pv_api_op;
 iv_mem_rec_t *ps_mem_rec;
            WORD32 max_wd = ps_ip->s_ivd_init_ip_t.u4_frm_max_wd;
            WORD32 max_ht = ps_ip->s_ivd_init_ip_t.u4_frm_max_ht;

            max_wd = ALIGN16(max_wd);
            max_ht = ALIGN32(max_ht);

            ps_op->s_ivd_init_op_t.u4_error_code = 0;

 if((ps_ip->s_ivd_init_ip_t.u4_size > sizeof(ih264d_init_ip_t))
 || (ps_ip->s_ivd_init_ip_t.u4_size
 < sizeof(ivd_init_ip_t)))
 {
                ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_init_op_t.u4_error_code |=
                                IVD_IP_API_STRUCT_SIZE_INCORRECT;
                H264_DEC_DEBUG_PRINT("\n");
 return (IV_FAIL);
 }

 if((ps_op->s_ivd_init_op_t.u4_size != sizeof(ih264d_init_op_t))
 && (ps_op->s_ivd_init_op_t.u4_size
 != sizeof(ivd_init_op_t)))
 {
                ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_init_op_t.u4_error_code |=
                                IVD_OP_API_STRUCT_SIZE_INCORRECT;
                H264_DEC_DEBUG_PRINT("\n");
 return (IV_FAIL);
 }

 if(ps_ip->s_ivd_init_ip_t.u4_num_mem_rec != MEM_REC_CNT)
 {
                ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_init_op_t.u4_error_code |=
                                IVD_INIT_DEC_NOT_SUFFICIENT;
                H264_DEC_DEBUG_PRINT("\n");
 return (IV_FAIL);
 }

 if(max_wd < H264_MIN_FRAME_WIDTH)
 {
                ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_init_op_t.u4_error_code |=
                                IVD_INIT_DEC_WIDTH_NOT_SUPPPORTED;
                H264_DEC_DEBUG_PRINT("\n");
 return (IV_FAIL);
 }

 if(max_wd > H264_MAX_FRAME_WIDTH)
 {
                ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_init_op_t.u4_error_code |=
                                IVD_INIT_DEC_WIDTH_NOT_SUPPPORTED;
                H264_DEC_DEBUG_PRINT("\n");
 return (IV_FAIL);
 }

 if(max_ht < H264_MIN_FRAME_HEIGHT)
 {
                ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_init_op_t.u4_error_code |=
                                IVD_INIT_DEC_HEIGHT_NOT_SUPPPORTED;
                H264_DEC_DEBUG_PRINT("\n");
 return (IV_FAIL);
 }

 if((max_ht * max_wd)
 > (H264_MAX_FRAME_HEIGHT * H264_MAX_FRAME_WIDTH))

 {
                ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_init_op_t.u4_error_code |=
                                IVD_INIT_DEC_HEIGHT_NOT_SUPPPORTED;
                H264_DEC_DEBUG_PRINT("\n");
 return (IV_FAIL);
 }

 if(NULL == ps_ip->s_ivd_init_ip_t.pv_mem_rec_location)
 {
                ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_init_op_t.u4_error_code |=
                                IVD_NUM_REC_NOT_SUFFICIENT;
                H264_DEC_DEBUG_PRINT("\n");
 return (IV_FAIL);
 }

 if((ps_ip->s_ivd_init_ip_t.e_output_format != IV_YUV_420P)
 && (ps_ip->s_ivd_init_ip_t.e_output_format
 != IV_YUV_422ILE)
 && (ps_ip->s_ivd_init_ip_t.e_output_format
 != IV_RGB_565)
 && (ps_ip->s_ivd_init_ip_t.e_output_format
 != IV_YUV_420SP_UV)
 && (ps_ip->s_ivd_init_ip_t.e_output_format
 != IV_YUV_420SP_VU))
 {
                ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_init_op_t.u4_error_code |=
                                IVD_INIT_DEC_COL_FMT_NOT_SUPPORTED;
                H264_DEC_DEBUG_PRINT("\n");
 return (IV_FAIL);
 }

  
 if(ps_ip->s_ivd_init_ip_t.u4_num_mem_rec < MEM_REC_CNT)
 {
                ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_init_op_t.u4_error_code |=
                                IVD_INIT_DEC_MEM_REC_NOT_SUFFICIENT;
                H264_DEC_DEBUG_PRINT("\n");
 return IV_FAIL;
 }

            ps_mem_rec = ps_ip->s_ivd_init_ip_t.pv_mem_rec_location;
  
 for(i = 0; i < ps_ip->s_ivd_init_ip_t.u4_num_mem_rec; i++)
 {
 if(ps_mem_rec[i].u4_size != sizeof(iv_mem_rec_t))
 {
                    ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_init_op_t.u4_error_code |=
                                    IVD_MEM_REC_STRUCT_SIZE_INCORRECT;
                    H264_DEC_DEBUG_PRINT("i: %d\n", i);
 return IV_FAIL;
 }
  

 if(ps_mem_rec[i].pv_base == NULL)
 {

                    ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_init_op_t.u4_error_code |=
                                    IVD_INIT_DEC_MEM_REC_BASE_NULL;
                    H264_DEC_DEBUG_PRINT("i: %d\n", i);
 return IV_FAIL;

 }

 }

  
 {
 void *start[MEM_REC_CNT];
 void *end[MEM_REC_CNT];

                start[0] = (void *)(ps_mem_rec[0].pv_base);
                end[0] = (void *)((UWORD8 *)ps_mem_rec[0].pv_base
 + ps_mem_rec[0].u4_mem_size - 1);
 for(i = 1; i < MEM_REC_CNT; i++)
 {
  
                    start[i] = (void *)(ps_mem_rec[i].pv_base);
                    end[i] = (void *)((UWORD8 *)ps_mem_rec[i].pv_base
 + ps_mem_rec[i].u4_mem_size - 1);

 for(j = 0; j < i; j++)
 {
 if((start[i] >= start[j]) && (start[i] <= end[j]))
 {
                            ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                            ps_op->s_ivd_init_op_t.u4_error_code |=
                                            IVD_INIT_DEC_MEM_REC_OVERLAP_ERR;
                            H264_DEC_DEBUG_PRINT("i: %d, j: %d\n", i, j);
 return IV_FAIL;
 }

 if((end[i] >= start[j]) && (end[i] <= end[j]))
 {
                            ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                            ps_op->s_ivd_init_op_t.u4_error_code |=
                                            IVD_INIT_DEC_MEM_REC_OVERLAP_ERR;
                            H264_DEC_DEBUG_PRINT("i: %d, j: %d\n", i, j);
 return IV_FAIL;
 }

 if((start[i] < start[j]) && (end[i] > end[j]))
 {
                            ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                            ps_op->s_ivd_init_op_t.u4_error_code |=
                                            IVD_INIT_DEC_MEM_REC_OVERLAP_ERR;
                            H264_DEC_DEBUG_PRINT("i: %d, j: %d\n", i, j);
 return IV_FAIL;
 }
 }

 }
 }

 {
 iv_mem_rec_t mem_rec_ittiam_api[MEM_REC_CNT];
 ih264d_fill_mem_rec_ip_t s_fill_mem_rec_ip;
 ih264d_fill_mem_rec_op_t s_fill_mem_rec_op;
                IV_API_CALL_STATUS_T e_status;

                UWORD32 i;
                s_fill_mem_rec_ip.s_ivd_fill_mem_rec_ip_t.e_cmd =
                                IV_CMD_FILL_NUM_MEM_REC;
                s_fill_mem_rec_ip.s_ivd_fill_mem_rec_ip_t.pv_mem_rec_location =
                                mem_rec_ittiam_api;
                s_fill_mem_rec_ip.s_ivd_fill_mem_rec_ip_t.u4_max_frm_wd =
                                max_wd;
                s_fill_mem_rec_ip.s_ivd_fill_mem_rec_ip_t.u4_max_frm_ht =
                                max_ht;

 if(ps_ip->s_ivd_init_ip_t.u4_size
 > offsetof(ih264d_init_ip_t, i4_level))
 {
                    s_fill_mem_rec_ip.i4_level = ps_ip->i4_level;
 }
 else
 {
                    s_fill_mem_rec_ip.i4_level = H264_LEVEL_3_1;
 }

 if(ps_ip->s_ivd_init_ip_t.u4_size
 > offsetof(ih264d_init_ip_t, u4_num_ref_frames))
 {
                    s_fill_mem_rec_ip.u4_num_ref_frames =
                                    ps_ip->u4_num_ref_frames;
 }
 else
 {
                    s_fill_mem_rec_ip.u4_num_ref_frames =
 (H264_MAX_REF_PICS + 1);
 }

 if(ps_ip->s_ivd_init_ip_t.u4_size
 > offsetof(ih264d_init_ip_t,
                                           u4_num_reorder_frames))
 {
                    s_fill_mem_rec_ip.u4_num_reorder_frames =
                                    ps_ip->u4_num_reorder_frames;
 }
 else
 {
                    s_fill_mem_rec_ip.u4_num_reorder_frames = (H264_MAX_REF_PICS
 + 1);
 }

 if(ps_ip->s_ivd_init_ip_t.u4_size
 > offsetof(ih264d_init_ip_t,
                                           u4_num_extra_disp_buf))
 {
                    s_fill_mem_rec_ip.u4_num_extra_disp_buf =
                                    ps_ip->u4_num_extra_disp_buf;
 }
 else
 {
                    s_fill_mem_rec_ip.u4_num_extra_disp_buf = 0;
 }

 if(ps_ip->s_ivd_init_ip_t.u4_size
 > offsetof(ih264d_init_ip_t, u4_share_disp_buf))
 {
#ifndef LOGO_EN
                    s_fill_mem_rec_ip.u4_share_disp_buf =
                                    ps_ip->u4_share_disp_buf;
#else
                    s_fill_mem_rec_ip.u4_share_disp_buf = 0;
#endif
 }
 else
 {
                    s_fill_mem_rec_ip.u4_share_disp_buf = 0;
 }

                s_fill_mem_rec_ip.e_output_format =
                                ps_ip->s_ivd_init_ip_t.e_output_format;

 if((s_fill_mem_rec_ip.e_output_format != IV_YUV_420P)
 && (s_fill_mem_rec_ip.e_output_format
 != IV_YUV_420SP_UV)
 && (s_fill_mem_rec_ip.e_output_format
 != IV_YUV_420SP_VU))
 {
                    s_fill_mem_rec_ip.u4_share_disp_buf = 0;
 }

                s_fill_mem_rec_ip.s_ivd_fill_mem_rec_ip_t.u4_size =
 sizeof(ih264d_fill_mem_rec_ip_t);
                s_fill_mem_rec_op.s_ivd_fill_mem_rec_op_t.u4_size =
 sizeof(ih264d_fill_mem_rec_op_t);

 for(i = 0; i < MEM_REC_CNT; i++)
                    mem_rec_ittiam_api[i].u4_size = sizeof(iv_mem_rec_t);

                e_status = ih264d_api_function(NULL,
 (void *)&s_fill_mem_rec_ip,
 (void *)&s_fill_mem_rec_op);
 if(IV_FAIL == e_status)
 {
                    ps_op->s_ivd_init_op_t.u4_error_code =
                                    s_fill_mem_rec_op.s_ivd_fill_mem_rec_op_t.u4_error_code;
                    H264_DEC_DEBUG_PRINT("Fail\n");
 return (IV_FAIL);
 }

 for(i = 0; i < MEM_REC_CNT; i++)
 {
 if(ps_mem_rec[i].u4_mem_size
 < mem_rec_ittiam_api[i].u4_mem_size)
 {
                        ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_init_op_t.u4_error_code |=
                                        IVD_INIT_DEC_MEM_REC_INSUFFICIENT_SIZE;
                        H264_DEC_DEBUG_PRINT("i: %d \n", i);
 return IV_FAIL;
 }
 if(ps_mem_rec[i].u4_mem_alignment
 != mem_rec_ittiam_api[i].u4_mem_alignment)
 {
                        ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_init_op_t.u4_error_code |=
                                        IVD_INIT_DEC_MEM_REC_ALIGNMENT_ERR;
                        H264_DEC_DEBUG_PRINT("i: %d \n", i);
 return IV_FAIL;
 }
 if(ps_mem_rec[i].e_mem_type
 != mem_rec_ittiam_api[i].e_mem_type)
 {
                        UWORD32 check = IV_SUCCESS;
                        UWORD32 diff = mem_rec_ittiam_api[i].e_mem_type
 - ps_mem_rec[i].e_mem_type;

 if((ps_mem_rec[i].e_mem_type
 <= IV_EXTERNAL_CACHEABLE_SCRATCH_MEM)
 && (mem_rec_ittiam_api[i].e_mem_type
 >= IV_INTERNAL_NONCACHEABLE_PERSISTENT_MEM))
 {
                            check = IV_FAIL;
 }
 if(3 != MOD(mem_rec_ittiam_api[i].e_mem_type, 4))
 {
  
 if((diff < 1) || (diff > 3))
 {
                                check = IV_FAIL;
 }
 }
 else
 {
 if(diff == 1)
 {
  
                                check = IV_FAIL;
 }
 if((diff != 2) && (diff != 3))
 {
                                check = IV_FAIL;
 }
 }
 if(check == IV_FAIL)
 {
                            ps_op->s_ivd_init_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                            ps_op->s_ivd_init_op_t.u4_error_code |=
                                            IVD_INIT_DEC_MEM_REC_INCORRECT_TYPE;
                            H264_DEC_DEBUG_PRINT("i: %d \n", i);
 return IV_FAIL;
 }
 }
 }
 }

 }
 break;

 case IVD_CMD_GET_DISPLAY_FRAME:
 {
 ih264d_get_display_frame_ip_t *ps_ip =
 (ih264d_get_display_frame_ip_t *)pv_api_ip;
 ih264d_get_display_frame_op_t *ps_op =
 (ih264d_get_display_frame_op_t *)pv_api_op;

            ps_op->s_ivd_get_display_frame_op_t.u4_error_code = 0;

 if((ps_ip->s_ivd_get_display_frame_ip_t.u4_size
 != sizeof(ih264d_get_display_frame_ip_t))
 && (ps_ip->s_ivd_get_display_frame_ip_t.u4_size
 != sizeof(ivd_get_display_frame_ip_t)))
 {
                ps_op->s_ivd_get_display_frame_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_get_display_frame_op_t.u4_error_code |=
                                IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

 if((ps_op->s_ivd_get_display_frame_op_t.u4_size
 != sizeof(ih264d_get_display_frame_op_t))
 && (ps_op->s_ivd_get_display_frame_op_t.u4_size
 != sizeof(ivd_get_display_frame_op_t)))
 {
                ps_op->s_ivd_get_display_frame_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_get_display_frame_op_t.u4_error_code |=
                                IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }
 }
 break;

 case IVD_CMD_REL_DISPLAY_FRAME:
 {
 ih264d_rel_display_frame_ip_t *ps_ip =
 (ih264d_rel_display_frame_ip_t *)pv_api_ip;
 ih264d_rel_display_frame_op_t *ps_op =
 (ih264d_rel_display_frame_op_t *)pv_api_op;

            ps_op->s_ivd_rel_display_frame_op_t.u4_error_code = 0;

 if((ps_ip->s_ivd_rel_display_frame_ip_t.u4_size
 != sizeof(ih264d_rel_display_frame_ip_t))
 && (ps_ip->s_ivd_rel_display_frame_ip_t.u4_size
 != sizeof(ivd_rel_display_frame_ip_t)))
 {
                ps_op->s_ivd_rel_display_frame_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_rel_display_frame_op_t.u4_error_code |=
                                IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

 if((ps_op->s_ivd_rel_display_frame_op_t.u4_size
 != sizeof(ih264d_rel_display_frame_op_t))
 && (ps_op->s_ivd_rel_display_frame_op_t.u4_size
 != sizeof(ivd_rel_display_frame_op_t)))
 {
                ps_op->s_ivd_rel_display_frame_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_rel_display_frame_op_t.u4_error_code |=
                                IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

 }
 break;

 case IVD_CMD_SET_DISPLAY_FRAME:
 {
 ih264d_set_display_frame_ip_t *ps_ip =
 (ih264d_set_display_frame_ip_t *)pv_api_ip;
 ih264d_set_display_frame_op_t *ps_op =
 (ih264d_set_display_frame_op_t *)pv_api_op;
            UWORD32 j;

            ps_op->s_ivd_set_display_frame_op_t.u4_error_code = 0;

 if((ps_ip->s_ivd_set_display_frame_ip_t.u4_size
 != sizeof(ih264d_set_display_frame_ip_t))
 && (ps_ip->s_ivd_set_display_frame_ip_t.u4_size
 != sizeof(ivd_set_display_frame_ip_t)))
 {
                ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_set_display_frame_op_t.u4_error_code |=
                                IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

 if((ps_op->s_ivd_set_display_frame_op_t.u4_size
 != sizeof(ih264d_set_display_frame_op_t))
 && (ps_op->s_ivd_set_display_frame_op_t.u4_size
 != sizeof(ivd_set_display_frame_op_t)))
 {
                ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_set_display_frame_op_t.u4_error_code |=
                                IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

 if(ps_ip->s_ivd_set_display_frame_ip_t.num_disp_bufs == 0)
 {
                ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_set_display_frame_op_t.u4_error_code |=
                                IVD_DISP_FRM_ZERO_OP_BUFS;
 return IV_FAIL;
 }

 for(j = 0; j < ps_ip->s_ivd_set_display_frame_ip_t.num_disp_bufs;
                            j++)
 {
 if(ps_ip->s_ivd_set_display_frame_ip_t.s_disp_buffer[j].u4_num_bufs
 == 0)
 {
                    ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_set_display_frame_op_t.u4_error_code |=
                                    IVD_DISP_FRM_ZERO_OP_BUFS;
 return IV_FAIL;
 }

 for(i = 0;
                                i
 < ps_ip->s_ivd_set_display_frame_ip_t.s_disp_buffer[j].u4_num_bufs;
                                i++)
 {
 if(ps_ip->s_ivd_set_display_frame_ip_t.s_disp_buffer[j].pu1_bufs[i]
 == NULL)
 {
                        ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_set_display_frame_op_t.u4_error_code |=
                                        IVD_DISP_FRM_OP_BUF_NULL;
 return IV_FAIL;
 }

 if(ps_ip->s_ivd_set_display_frame_ip_t.s_disp_buffer[j].u4_min_out_buf_size[i]
 == 0)
 {
                        ps_op->s_ivd_set_display_frame_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_set_display_frame_op_t.u4_error_code |=
                                        IVD_DISP_FRM_ZERO_OP_BUF_SIZE;
 return IV_FAIL;
 }
 }
 }
 }
 break;

 case IVD_CMD_VIDEO_DECODE:
 {
 ih264d_video_decode_ip_t *ps_ip =
 (ih264d_video_decode_ip_t *)pv_api_ip;
 ih264d_video_decode_op_t *ps_op =
 (ih264d_video_decode_op_t *)pv_api_op;

            H264_DEC_DEBUG_PRINT("The input bytes is: %d",
                                 ps_ip->s_ivd_video_decode_ip_t.u4_num_Bytes);
            ps_op->s_ivd_video_decode_op_t.u4_error_code = 0;

 if(ps_ip->s_ivd_video_decode_ip_t.u4_size
 != sizeof(ih264d_video_decode_ip_t)&&
                            ps_ip->s_ivd_video_decode_ip_t.u4_size != offsetof(ivd_video_decode_ip_t, s_out_buffer))
 {
                ps_op->s_ivd_video_decode_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_video_decode_op_t.u4_error_code |=
                                IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

 if(ps_op->s_ivd_video_decode_op_t.u4_size
 != sizeof(ih264d_video_decode_op_t)&&
                            ps_op->s_ivd_video_decode_op_t.u4_size != offsetof(ivd_video_decode_op_t, u4_output_present))
 {
                ps_op->s_ivd_video_decode_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_video_decode_op_t.u4_error_code |=
                                IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

 }
 break;

 case IV_CMD_RETRIEVE_MEMREC:
 {
 ih264d_retrieve_mem_rec_ip_t *ps_ip =
 (ih264d_retrieve_mem_rec_ip_t *)pv_api_ip;
 ih264d_retrieve_mem_rec_op_t *ps_op =
 (ih264d_retrieve_mem_rec_op_t *)pv_api_op;
 iv_mem_rec_t *ps_mem_rec;

            ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code = 0;

 if(ps_ip->s_ivd_retrieve_mem_rec_ip_t.u4_size
 != sizeof(ih264d_retrieve_mem_rec_ip_t))
 {
                ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code |=
                                IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

 if(ps_op->s_ivd_retrieve_mem_rec_op_t.u4_size
 != sizeof(ih264d_retrieve_mem_rec_op_t))
 {
                ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code |=
                                IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

            ps_mem_rec = ps_ip->s_ivd_retrieve_mem_rec_ip_t.pv_mem_rec_location;
  
 for(i = 0; i < MEM_REC_CNT; i++)
 {
 if(ps_mem_rec[i].u4_size != sizeof(iv_mem_rec_t))
 {
                    ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                    ps_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code |=
                                    IVD_MEM_REC_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 }
 break;

 case IVD_CMD_VIDEO_CTL:
 {
            UWORD32 *pu4_ptr_cmd;
            UWORD32 sub_command;

            pu4_ptr_cmd = (UWORD32 *)pv_api_ip;
            pu4_ptr_cmd += 2;
            sub_command = *pu4_ptr_cmd;

 switch(sub_command)
 {
 case IVD_CMD_CTL_SETPARAMS:
 {
 ih264d_ctl_set_config_ip_t *ps_ip;
 ih264d_ctl_set_config_op_t *ps_op;
                    ps_ip = (ih264d_ctl_set_config_ip_t *)pv_api_ip;
                    ps_op = (ih264d_ctl_set_config_op_t *)pv_api_op;

 if(ps_ip->s_ivd_ctl_set_config_ip_t.u4_size
 != sizeof(ih264d_ctl_set_config_ip_t))
 {
                        ps_op->s_ivd_ctl_set_config_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_ctl_set_config_op_t.u4_error_code |=
                                        IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 case IVD_CMD_CTL_SETDEFAULT:
 {
 ih264d_ctl_set_config_op_t *ps_op;
                    ps_op = (ih264d_ctl_set_config_op_t *)pv_api_op;
 if(ps_op->s_ivd_ctl_set_config_op_t.u4_size
 != sizeof(ih264d_ctl_set_config_op_t))
 {
                        ps_op->s_ivd_ctl_set_config_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_ctl_set_config_op_t.u4_error_code |=
                                        IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 break;

 case IVD_CMD_CTL_GETPARAMS:
 {
 ih264d_ctl_getstatus_ip_t *ps_ip;
 ih264d_ctl_getstatus_op_t *ps_op;

                    ps_ip = (ih264d_ctl_getstatus_ip_t *)pv_api_ip;
                    ps_op = (ih264d_ctl_getstatus_op_t *)pv_api_op;
 if(ps_ip->s_ivd_ctl_getstatus_ip_t.u4_size
 != sizeof(ih264d_ctl_getstatus_ip_t))
 {
                        ps_op->s_ivd_ctl_getstatus_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_ctl_getstatus_op_t.u4_error_code |=
                                        IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 if(ps_op->s_ivd_ctl_getstatus_op_t.u4_size
 != sizeof(ih264d_ctl_getstatus_op_t))
 {
                        ps_op->s_ivd_ctl_getstatus_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_ctl_getstatus_op_t.u4_error_code |=
                                        IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 break;

 case IVD_CMD_CTL_GETBUFINFO:
 {
 ih264d_ctl_getbufinfo_ip_t *ps_ip;
 ih264d_ctl_getbufinfo_op_t *ps_op;
                    ps_ip = (ih264d_ctl_getbufinfo_ip_t *)pv_api_ip;
                    ps_op = (ih264d_ctl_getbufinfo_op_t *)pv_api_op;

 if(ps_ip->s_ivd_ctl_getbufinfo_ip_t.u4_size
 != sizeof(ih264d_ctl_getbufinfo_ip_t))
 {
                        ps_op->s_ivd_ctl_getbufinfo_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_ctl_getbufinfo_op_t.u4_error_code |=
                                        IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 if(ps_op->s_ivd_ctl_getbufinfo_op_t.u4_size
 != sizeof(ih264d_ctl_getbufinfo_op_t))
 {
                        ps_op->s_ivd_ctl_getbufinfo_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_ctl_getbufinfo_op_t.u4_error_code |=
                                        IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 break;

 case IVD_CMD_CTL_GETVERSION:
 {
 ih264d_ctl_getversioninfo_ip_t *ps_ip;
 ih264d_ctl_getversioninfo_op_t *ps_op;
                    ps_ip = (ih264d_ctl_getversioninfo_ip_t *)pv_api_ip;
                    ps_op = (ih264d_ctl_getversioninfo_op_t *)pv_api_op;
 if(ps_ip->s_ivd_ctl_getversioninfo_ip_t.u4_size
 != sizeof(ih264d_ctl_getversioninfo_ip_t))
 {
                        ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code |=
                                        IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 if(ps_op->s_ivd_ctl_getversioninfo_op_t.u4_size
 != sizeof(ih264d_ctl_getversioninfo_op_t))
 {
                        ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_ctl_getversioninfo_op_t.u4_error_code |=
                                        IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 break;

 case IVD_CMD_CTL_FLUSH:
 {
 ih264d_ctl_flush_ip_t *ps_ip;
 ih264d_ctl_flush_op_t *ps_op;
                    ps_ip = (ih264d_ctl_flush_ip_t *)pv_api_ip;
                    ps_op = (ih264d_ctl_flush_op_t *)pv_api_op;
 if(ps_ip->s_ivd_ctl_flush_ip_t.u4_size
 != sizeof(ih264d_ctl_flush_ip_t))
 {
                        ps_op->s_ivd_ctl_flush_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_ctl_flush_op_t.u4_error_code |=
                                        IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 if(ps_op->s_ivd_ctl_flush_op_t.u4_size
 != sizeof(ih264d_ctl_flush_op_t))
 {
                        ps_op->s_ivd_ctl_flush_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_ctl_flush_op_t.u4_error_code |=
                                        IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 break;

 case IVD_CMD_CTL_RESET:
 {
 ih264d_ctl_reset_ip_t *ps_ip;
 ih264d_ctl_reset_op_t *ps_op;
                    ps_ip = (ih264d_ctl_reset_ip_t *)pv_api_ip;
                    ps_op = (ih264d_ctl_reset_op_t *)pv_api_op;
 if(ps_ip->s_ivd_ctl_reset_ip_t.u4_size
 != sizeof(ih264d_ctl_reset_ip_t))
 {
                        ps_op->s_ivd_ctl_reset_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_ctl_reset_op_t.u4_error_code |=
                                        IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 if(ps_op->s_ivd_ctl_reset_op_t.u4_size
 != sizeof(ih264d_ctl_reset_op_t))
 {
                        ps_op->s_ivd_ctl_reset_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                        ps_op->s_ivd_ctl_reset_op_t.u4_error_code |=
                                        IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }
 }
 break;

 case IH264D_CMD_CTL_DEGRADE:
 {
 ih264d_ctl_degrade_ip_t *ps_ip;
 ih264d_ctl_degrade_op_t *ps_op;

                    ps_ip = (ih264d_ctl_degrade_ip_t *)pv_api_ip;
                    ps_op = (ih264d_ctl_degrade_op_t *)pv_api_op;

 if(ps_ip->u4_size != sizeof(ih264d_ctl_degrade_ip_t))
 {
                        ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->u4_error_code |=
                                        IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 if(ps_op->u4_size != sizeof(ih264d_ctl_degrade_op_t))
 {
                        ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->u4_error_code |=
                                        IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 if((ps_ip->i4_degrade_pics < 0)
 || (ps_ip->i4_degrade_pics > 4)
 || (ps_ip->i4_nondegrade_interval < 0)
 || (ps_ip->i4_degrade_type < 0)
 || (ps_ip->i4_degrade_type > 15))
 {
                        ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
 return IV_FAIL;
 }

 break;
 }

 case IH264D_CMD_CTL_GET_BUFFER_DIMENSIONS:
 {
 ih264d_ctl_get_frame_dimensions_ip_t *ps_ip;
 ih264d_ctl_get_frame_dimensions_op_t *ps_op;

                    ps_ip = (ih264d_ctl_get_frame_dimensions_ip_t *)pv_api_ip;
                    ps_op = (ih264d_ctl_get_frame_dimensions_op_t *)pv_api_op;

 if(ps_ip->u4_size
 != sizeof(ih264d_ctl_get_frame_dimensions_ip_t))
 {
                        ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->u4_error_code |=
                                        IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 if(ps_op->u4_size
 != sizeof(ih264d_ctl_get_frame_dimensions_op_t))
 {
                        ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->u4_error_code |=
                                        IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 break;
 }

 case IH264D_CMD_CTL_SET_NUM_CORES:
 {
 ih264d_ctl_set_num_cores_ip_t *ps_ip;
 ih264d_ctl_set_num_cores_op_t *ps_op;

                    ps_ip = (ih264d_ctl_set_num_cores_ip_t *)pv_api_ip;
                    ps_op = (ih264d_ctl_set_num_cores_op_t *)pv_api_op;

 if(ps_ip->u4_size != sizeof(ih264d_ctl_set_num_cores_ip_t))
 {
                        ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->u4_error_code |=
                                        IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 if(ps_op->u4_size != sizeof(ih264d_ctl_set_num_cores_op_t))
 {
                        ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->u4_error_code |=
                                        IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 if((ps_ip->u4_num_cores != 1) && (ps_ip->u4_num_cores != 2)
 && (ps_ip->u4_num_cores != 3)
 && (ps_ip->u4_num_cores != 4))
 {
                        ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
 return IV_FAIL;
 }
 break;
 }
 case IH264D_CMD_CTL_SET_PROCESSOR:
 {
 ih264d_ctl_set_processor_ip_t *ps_ip;
 ih264d_ctl_set_processor_op_t *ps_op;

                    ps_ip = (ih264d_ctl_set_processor_ip_t *)pv_api_ip;
                    ps_op = (ih264d_ctl_set_processor_op_t *)pv_api_op;

 if(ps_ip->u4_size != sizeof(ih264d_ctl_set_processor_ip_t))
 {
                        ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->u4_error_code |=
                                        IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 if(ps_op->u4_size != sizeof(ih264d_ctl_set_processor_op_t))
 {
                        ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->u4_error_code |=
                                        IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 break;
 }
 default:
 *(pu4_api_op + 1) |= 1 << IVD_UNSUPPORTEDPARAM;
 *(pu4_api_op + 1) |= IVD_UNSUPPORTED_API_CMD;
 return IV_FAIL;
 break;
 }
 }
 break;
 }

 return IV_SUCCESS;
}
