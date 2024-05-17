void btm_sec_rmt_host_support_feat_evt (UINT8 *p)
{
    tBTM_SEC_DEV_REC *p_dev_rec;
    BD_ADDR         bd_addr;  
    BD_FEATURES     features;

    STREAM_TO_BDADDR (bd_addr, p);
    p_dev_rec = btm_find_or_alloc_dev (bd_addr);

    BTM_TRACE_EVENT ("btm_sec_rmt_host_support_feat_evt  sm4: 0x%x  p[0]: 0x%x", p_dev_rec->sm4, p[0]);

 if (BTM_SEC_IS_SM4_UNKNOWN(p_dev_rec->sm4))
 {
        p_dev_rec->sm4 = BTM_SM4_KNOWN;
        STREAM_TO_ARRAY(features, p, HCI_FEATURE_BYTES_PER_PAGE);
 if (HCI_SSP_HOST_SUPPORTED(features))
 {
            p_dev_rec->sm4 = BTM_SM4_TRUE;
 }
        BTM_TRACE_EVENT ("btm_sec_rmt_host_support_feat_evt sm4: 0x%x features[0]: 0x%x", p_dev_rec->sm4, features[0]);
 }
}
