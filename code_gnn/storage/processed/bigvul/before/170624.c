int Reverb_process(effect_handle_t   self,
 audio_buffer_t *inBuffer,
 audio_buffer_t *outBuffer){
    android::ReverbContext * pContext = (android::ReverbContext *) self;
 int    status = 0;

 if (pContext == NULL){
        ALOGV("\tLVM_ERROR : Reverb_process() ERROR pContext == NULL");
 return -EINVAL;
 }
 if (inBuffer == NULL  || inBuffer->raw == NULL  ||
            outBuffer == NULL || outBuffer->raw == NULL ||
            inBuffer->frameCount != outBuffer->frameCount){
        ALOGV("\tLVM_ERROR : Reverb_process() ERROR NULL INPUT POINTER OR FRAME COUNT IS WRONG");
 return -EINVAL;
 }
  
    status = process( (LVM_INT16 *)inBuffer->raw,
 (LVM_INT16 *)outBuffer->raw,
                                      outBuffer->frameCount,
                                      pContext);

 if (pContext->bEnabled == LVM_FALSE) {
 if (pContext->SamplesToExitCount > 0) {
            pContext->SamplesToExitCount -= outBuffer->frameCount;
 } else {
            status = -ENODATA;
 }
 }

 return status;
}  
