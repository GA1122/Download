u32 h264bsdProfile(storage_t *pStorage)
{
 if (pStorage->activeSps)
 return pStorage->activeSps->profileIdc;
 else
 return 0;
}
