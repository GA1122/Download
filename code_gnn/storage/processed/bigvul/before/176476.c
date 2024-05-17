IV_API_CALL_STATUS_T ihevcd_cxa_api_function(iv_obj_t *ps_handle,
 void *pv_api_ip,
 void *pv_api_op)
{
    WORD32 command;
    UWORD32 *pu4_ptr_cmd;
    WORD32 ret = 0;
    IV_API_CALL_STATUS_T e_status;
    e_status = api_check_struct_sanity(ps_handle, pv_api_ip, pv_api_op);

 if(e_status != IV_SUCCESS)
 {
        DEBUG("error code = %d\n", *((UWORD32 *)pv_api_op + 1));
 return IV_FAIL;
 }

    pu4_ptr_cmd = (UWORD32 *)pv_api_ip;
    pu4_ptr_cmd++;

    command = *pu4_ptr_cmd;

 switch(command)
 {
 case IVD_CMD_CREATE:
            ret = ihevcd_create(ps_handle, (void *)pv_api_ip, (void *)pv_api_op);
 break;
 case IVD_CMD_DELETE:
            ret = ihevcd_delete(ps_handle, (void *)pv_api_ip, (void *)pv_api_op);
 break;

 case IVD_CMD_VIDEO_DECODE:
            ret = ihevcd_decode(ps_handle, (void *)pv_api_ip, (void *)pv_api_op);
 break;

 case IVD_CMD_GET_DISPLAY_FRAME:
 break;

 case IVD_CMD_SET_DISPLAY_FRAME:
            ret = ihevcd_set_display_frame(ps_handle, (void *)pv_api_ip,
 (void *)pv_api_op);

 break;

 case IVD_CMD_REL_DISPLAY_FRAME:
            ret = ihevcd_rel_display_frame(ps_handle, (void *)pv_api_ip,
 (void *)pv_api_op);
 break;

 case IVD_CMD_VIDEO_CTL:
            ret = ihevcd_ctl(ps_handle, (void *)pv_api_ip, (void *)pv_api_op);
 break;
 default:
            ret = IV_FAIL;
 break;
 }

 return (IV_API_CALL_STATUS_T)ret;
}
