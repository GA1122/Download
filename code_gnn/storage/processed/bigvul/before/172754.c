dpbOutPicture_t* h264bsdDpbOutputPicture(dpbStorage_t *dpb)
{

 

 

    ASSERT(dpb);

 if (dpb->outIndex < dpb->numOut)
 return(dpb->outBuf + dpb->outIndex++);
 else
 return(NULL);

}
