int venc_dev::append_mbi_extradata(void *dst, struct msm_vidc_extradata_header* src)
{
    OMX_QCOM_EXTRADATA_MBINFO *mbi = (OMX_QCOM_EXTRADATA_MBINFO *)dst;

 if (!dst || !src)
 return 0;

  
    mbi->nFormat = 1;
    mbi->nDataSize = src->data_size;
    memcpy(&mbi->data, &src->data, src->data_size);

 return mbi->nDataSize + sizeof(*mbi);
}
