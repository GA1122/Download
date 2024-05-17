u8* h264bsdNextOutputPicture(storage_t *pStorage, u32 *picId, u32 *isIdrPic,
    u32 *numErrMbs)
{

 

 dpbOutPicture_t *pOut;

 

    ASSERT(pStorage);

    pOut = h264bsdDpbOutputPicture(pStorage->dpb);

 if (pOut != NULL)
 {
 *picId = pOut->picId;
 *isIdrPic = pOut->isIdr;
 *numErrMbs = pOut->numErrMbs;
 return (pOut->data);
 }
 else
 return(NULL);

}
