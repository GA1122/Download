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
 case IVD_CMD_CREATE:
 break;

 case IVD_CMD_REL_DISPLAY_FRAME:
 case IVD_CMD_SET_DISPLAY_FRAME:
 case IVD_CMD_GET_DISPLAY_FRAME:
 case IVD_CMD_VIDEO_DECODE:
 case IVD_CMD_DELETE:
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
 case IVD_CMD_CREATE:
 {
 ih264d_create_ip_t *ps_ip = (ih264d_create_ip_t *)pv_api_ip;
 ih264d_create_op_t *ps_op = (ih264d_create_op_t *)pv_api_op;


            ps_op->s_ivd_create_op_t.u4_error_code = 0;

 if((ps_ip->s_ivd_create_ip_t.u4_size > sizeof(ih264d_create_ip_t))
 || (ps_ip->s_ivd_create_ip_t.u4_size
 < sizeof(ivd_create_ip_t)))
 {
                ps_op->s_ivd_create_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_create_op_t.u4_error_code |=
                                IVD_IP_API_STRUCT_SIZE_INCORRECT;
                H264_DEC_DEBUG_PRINT("\n");
 return (IV_FAIL);
 }

 if((ps_op->s_ivd_create_op_t.u4_size != sizeof(ih264d_create_op_t))
 && (ps_op->s_ivd_create_op_t.u4_size
 != sizeof(ivd_create_op_t)))
 {
                ps_op->s_ivd_create_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_create_op_t.u4_error_code |=
                                IVD_OP_API_STRUCT_SIZE_INCORRECT;
                H264_DEC_DEBUG_PRINT("\n");
 return (IV_FAIL);
 }


 if((ps_ip->s_ivd_create_ip_t.e_output_format != IV_YUV_420P)
 && (ps_ip->s_ivd_create_ip_t.e_output_format
 != IV_YUV_422ILE)
 && (ps_ip->s_ivd_create_ip_t.e_output_format
 != IV_RGB_565)
 && (ps_ip->s_ivd_create_ip_t.e_output_format
 != IV_YUV_420SP_UV)
 && (ps_ip->s_ivd_create_ip_t.e_output_format
 != IV_YUV_420SP_VU))
 {
                ps_op->s_ivd_create_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_create_op_t.u4_error_code |=
                                IVD_INIT_DEC_COL_FMT_NOT_SUPPORTED;
                H264_DEC_DEBUG_PRINT("\n");
 return (IV_FAIL);
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

 case IVD_CMD_DELETE:
 {
 ih264d_delete_ip_t *ps_ip =
 (ih264d_delete_ip_t *)pv_api_ip;
 ih264d_delete_op_t *ps_op =
 (ih264d_delete_op_t *)pv_api_op;

            ps_op->s_ivd_delete_op_t.u4_error_code = 0;

 if(ps_ip->s_ivd_delete_ip_t.u4_size
 != sizeof(ih264d_delete_ip_t))
 {
                ps_op->s_ivd_delete_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_delete_op_t.u4_error_code |=
                                IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
 }

 if(ps_op->s_ivd_delete_op_t.u4_size
 != sizeof(ih264d_delete_op_t))
 {
                ps_op->s_ivd_delete_op_t.u4_error_code |= 1
 << IVD_UNSUPPORTEDPARAM;
                ps_op->s_ivd_delete_op_t.u4_error_code |=
                                IVD_OP_API_STRUCT_SIZE_INCORRECT;
 return (IV_FAIL);
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
 case IH264D_CMD_CTL_GET_VUI_PARAMS:
 {
 ih264d_ctl_get_vui_params_ip_t *ps_ip;
 ih264d_ctl_get_vui_params_op_t *ps_op;

                    ps_ip =
 (ih264d_ctl_get_vui_params_ip_t *)pv_api_ip;
                    ps_op =
 (ih264d_ctl_get_vui_params_op_t *)pv_api_op;

 if(ps_ip->u4_size
 != sizeof(ih264d_ctl_get_vui_params_ip_t))
 {
                        ps_op->u4_error_code |= 1 << IVD_UNSUPPORTEDPARAM;
                        ps_op->u4_error_code |=
                                        IVD_IP_API_STRUCT_SIZE_INCORRECT;
 return IV_FAIL;
 }

 if(ps_op->u4_size
 != sizeof(ih264d_ctl_get_vui_params_op_t))
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
