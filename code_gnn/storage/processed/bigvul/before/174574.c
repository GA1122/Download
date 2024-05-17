BOOLEAN BTM_SetUCDSecurityLevel (BOOLEAN is_originator, char *p_name, UINT8 service_id,
                                 UINT16 sec_level, UINT16 psm, UINT32 mx_proto_id,
                                 UINT32 mx_chan_id)
{
#if (L2CAP_UCD_INCLUDED == TRUE)
    CONNECTION_TYPE conn_type;

 if (is_originator)
        conn_type = CONNLESS_ORIG;
 else
        conn_type = CONNLESS_TERM;

 return(btm_sec_set_security_level (conn_type, p_name, service_id,
                                       sec_level, psm, mx_proto_id, mx_chan_id));
#else
    UNUSED(is_originator);
    UNUSED(p_name);
    UNUSED(service_id);
    UNUSED(sec_level);
    UNUSED(psm);
    UNUSED(mx_proto_id);
    UNUSED(mx_chan_id);
 return FALSE;
#endif
}
