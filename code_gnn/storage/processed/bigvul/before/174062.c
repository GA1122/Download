u32 h264bsdCheckValidParamSets(storage_t *pStorage)
{

 

 

    ASSERT(pStorage);

 return(h264bsdValidParamSets(pStorage) == HANTRO_OK ? 1 : 0);

}
