IV_API_CALL_STATUS_T impeg2d_api_function (iv_obj_t *ps_dechdl, void *pv_api_ip,void *pv_api_op)
{
    WORD32 i4_cmd;
    IV_API_CALL_STATUS_T u4_error_code;
    UWORD32 *pu4_api_ip;

    u4_error_code = impeg2d_api_check_struct_sanity(ps_dechdl,pv_api_ip,pv_api_op);
 if(IV_SUCCESS != u4_error_code)
 {
 return u4_error_code;
 }


    pu4_api_ip  = (UWORD32 *)pv_api_ip;
    i4_cmd = *(pu4_api_ip + 1);

 switch(i4_cmd)
 {

 case IV_CMD_GET_NUM_MEM_REC:
        u4_error_code = impeg2d_api_num_mem_rec((void *)pv_api_ip,(void *)pv_api_op);
 break;

 case IV_CMD_FILL_NUM_MEM_REC:
        u4_error_code = impeg2d_api_fill_mem_rec((void *)pv_api_ip,(void *)pv_api_op);
 break;

 case IV_CMD_INIT:
        u4_error_code = impeg2d_api_init(ps_dechdl,(void *)pv_api_ip,(void *)pv_api_op);
 break;

 case IVD_CMD_SET_DISPLAY_FRAME:
        u4_error_code = impeg2d_api_set_display_frame(ps_dechdl,(void *)pv_api_ip,(void *)pv_api_op);
 break;

 case IVD_CMD_REL_DISPLAY_FRAME:
        u4_error_code = impeg2d_api_rel_display_frame(ps_dechdl,(void *)pv_api_ip,(void *)pv_api_op);
 break;

 case IVD_CMD_VIDEO_DECODE:
        u4_error_code = impeg2d_api_entity(ps_dechdl, (void *)pv_api_ip,(void *)pv_api_op);
 break;

 case IV_CMD_RETRIEVE_MEMREC:
        u4_error_code = impeg2d_api_retrieve_mem_rec(ps_dechdl,(void *)pv_api_ip,(void *)pv_api_op);
 break;

 case IVD_CMD_VIDEO_CTL:
        u4_error_code = impeg2d_api_ctl(ps_dechdl,(void *)pv_api_ip,(void *)pv_api_op);
 break;

 default:
 break;
 }

 return(u4_error_code);

}
