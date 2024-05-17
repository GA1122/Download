bool omx_venc::dev_color_align(OMX_BUFFERHEADERTYPE *buffer,
                OMX_U32 width, OMX_U32 height)
{
 if(secure_session) {
        DEBUG_PRINT_ERROR("Cannot align colors in secure session.");
 return OMX_FALSE;
 }
 return handle->venc_color_align(buffer, width,height);
}
