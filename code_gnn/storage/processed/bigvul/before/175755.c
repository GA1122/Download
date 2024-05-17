IV_API_CALL_STATUS_T impeg2d_api_ctl(iv_obj_t *ps_dechdl,
 void *pv_api_ip,
 void *pv_api_op)
{
    WORD32 i4_sub_cmd;
    UWORD32 *pu4_api_ip;
    IV_API_CALL_STATUS_T u4_error_code;

    pu4_api_ip = (UWORD32 *)pv_api_ip;
    i4_sub_cmd = *(pu4_api_ip + 2);

 switch(i4_sub_cmd)
 {
 case IVD_CMD_CTL_GETPARAMS:
            u4_error_code = impeg2d_api_get_status(ps_dechdl, (void *)pv_api_ip,
 (void *)pv_api_op);
 break;

 case IVD_CMD_CTL_SETPARAMS:
            u4_error_code = impeg2d_api_set_params(ps_dechdl, (void *)pv_api_ip,
 (void *)pv_api_op);
 break;

 case IVD_CMD_CTL_RESET:
            u4_error_code = impeg2d_api_reset(ps_dechdl, (void *)pv_api_ip,
 (void *)pv_api_op);
 break;

 case IVD_CMD_CTL_SETDEFAULT:
            u4_error_code = impeg2d_api_set_default(ps_dechdl,
 (void *)pv_api_ip,
 (void *)pv_api_op);
 break;

 case IVD_CMD_CTL_FLUSH:
            u4_error_code = impeg2d_api_set_flush_mode(ps_dechdl,
 (void *)pv_api_ip,
 (void *)pv_api_op);
 break;

 case IVD_CMD_CTL_GETBUFINFO:
            u4_error_code = impeg2d_api_get_buf_info(ps_dechdl,
 (void *)pv_api_ip,
 (void *)pv_api_op);
 break;

 case IVD_CMD_CTL_GETVERSION:
            u4_error_code = impeg2d_api_get_version(ps_dechdl, (void *)pv_api_ip,
 (void *)pv_api_op);
 break;

 case IMPEG2D_CMD_CTL_SET_NUM_CORES:
            u4_error_code = impeg2d_api_set_num_cores(ps_dechdl,
 (void *)pv_api_ip,
 (void *)pv_api_op);
 break;

 case IMPEG2D_CMD_CTL_GET_BUFFER_DIMENSIONS:
            u4_error_code = impeg2d_get_frame_dimensions(ps_dechdl,
 (void *)pv_api_ip,
 (void *)pv_api_op);
 break;

 case IMPEG2D_CMD_CTL_GET_SEQ_INFO:
            u4_error_code = impeg2d_api_get_seq_info(ps_dechdl,
 (void *)pv_api_ip,
 (void *)pv_api_op);
 break;

 case IMPEG2D_CMD_CTL_SET_PROCESSOR:
            u4_error_code = impeg2d_set_processor(ps_dechdl, (void *)pv_api_ip,
 (void *)pv_api_op);
 break;

 default:
            u4_error_code = IV_FAIL;
 break;
 }

 return (u4_error_code);

}
