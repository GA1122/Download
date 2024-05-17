UINT16 BTM_BuildOobData(UINT8 *p_data, UINT16 max_len, BT_OCTET16 c,
                        BT_OCTET16 r, UINT8 name_len)
{
    UINT8   *p = p_data;
    UINT16  len = 0;
    UINT16  delta;
#if BTM_MAX_LOC_BD_NAME_LEN > 0
    UINT16  name_size;
    UINT8   name_type = BTM_EIR_SHORTENED_LOCAL_NAME_TYPE;
#endif

 if (p_data && max_len >= BTM_OOB_MANDATORY_SIZE)
 {
  
        UINT16_TO_STREAM(p, len);
        BDADDR_TO_STREAM(p, btm_cb.devcb.local_addr);

        len = BTM_OOB_MANDATORY_SIZE;
        max_len -= len;

  

  
        delta = BTM_OOB_HASH_C_SIZE + 2;
 if (max_len >= delta)
 {
 *p++ = BTM_OOB_HASH_C_SIZE + 1;
 *p++ = BTM_EIR_OOB_SSP_HASH_C_TYPE;
            ARRAY_TO_STREAM(p, c, BTM_OOB_HASH_C_SIZE);
            len     += delta;
            max_len -= delta;
 }

  
        delta = BTM_OOB_RAND_R_SIZE + 2;
 if (max_len >= delta)
 {
 *p++ = BTM_OOB_RAND_R_SIZE + 1;
 *p++ = BTM_EIR_OOB_SSP_RAND_R_TYPE;
            ARRAY_TO_STREAM(p, r, BTM_OOB_RAND_R_SIZE);
            len     += delta;
            max_len -= delta;
 }

  
        delta = BTM_OOB_COD_SIZE + 2;
 if (max_len >= delta)
 {
 *p++ = BTM_OOB_COD_SIZE + 1;
 *p++ = BTM_EIR_OOB_COD_TYPE;
            DEVCLASS_TO_STREAM(p, btm_cb.devcb.dev_class);
            len     += delta;
            max_len -= delta;
 }
#if BTM_MAX_LOC_BD_NAME_LEN > 0
        name_size = name_len;
 if (name_size > strlen(btm_cb.cfg.bd_name))
 {
            name_type = BTM_EIR_COMPLETE_LOCAL_NAME_TYPE;
            name_size = (UINT16)strlen(btm_cb.cfg.bd_name);
 }
        delta = name_size + 2;
 if (max_len >= delta)
 {
 *p++ = name_size + 1;
 *p++ = name_type;
            ARRAY_TO_STREAM (p, btm_cb.cfg.bd_name, name_size);
            len     += delta;
            max_len -= delta;
 }
#endif
  
        p = p_data;
        UINT16_TO_STREAM(p, len);
 }
 return len;
}
