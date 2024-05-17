OMX_ERRORTYPE SimpleSoftOMXComponent::allocateBuffer(
        OMX_BUFFERHEADERTYPE **header,
        OMX_U32 portIndex,
        OMX_PTR appPrivate,
        OMX_U32 size) {
    OMX_U8 *ptr = new OMX_U8[size];

    OMX_ERRORTYPE err =
        useBuffer(header, portIndex, appPrivate, size, ptr);

 if (err != OMX_ErrorNone) {
 delete[] ptr;
        ptr = NULL;

 return err;
 }

    CHECK((*header)->pPlatformPrivate == NULL);
 (*header)->pPlatformPrivate = ptr;

 return OMX_ErrorNone;
}
