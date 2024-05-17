OMX_ERRORTYPE  omx_vdec::use_EGL_image(OMX_IN OMX_HANDLETYPE     hComp,
        OMX_INOUT OMX_BUFFERHEADERTYPE** bufferHdr,
        OMX_IN OMX_U32                        port,
        OMX_IN OMX_PTR                     appData,
        OMX_IN void*                      eglImage)
{
 (void) appData;
    OMX_QCOM_PLATFORM_PRIVATE_LIST pmem_list;
    OMX_QCOM_PLATFORM_PRIVATE_ENTRY pmem_entry;
    OMX_QCOM_PLATFORM_PRIVATE_PMEM_INFO pmem_info;

#ifdef USE_EGL_IMAGE_GPU
    PFNEGLQUERYIMAGEQUALCOMMPROC egl_queryfunc;
 EGLint fd = -1, offset = 0,pmemPtr = 0;
#else
 int fd = -1, offset = 0;
#endif
    DEBUG_PRINT_HIGH("use EGL image support for decoder");
 if (!bufferHdr || !eglImage|| port != OMX_CORE_OUTPUT_PORT_INDEX) {
        DEBUG_PRINT_ERROR("Invalid EGL image");
 }
#ifdef USE_EGL_IMAGE_GPU
 if (m_display_id == NULL) {
        DEBUG_PRINT_ERROR("Display ID is not set by IL client");
 return OMX_ErrorInsufficientResources;
 }
    egl_queryfunc = (PFNEGLQUERYIMAGEQUALCOMMPROC)
        eglGetProcAddress("eglQueryImageKHR");
    egl_queryfunc(m_display_id, eglImage, EGL_BUFFER_HANDLE, &fd);
    egl_queryfunc(m_display_id, eglImage, EGL_BUFFER_OFFSET, &offset);
    egl_queryfunc(m_display_id, eglImage, EGL_BITMAP_POINTER_KHR,&pmemPtr);
#else  
 struct temp_egl {
 int pmem_fd;
 int offset;
 };
 struct temp_egl *temp_egl_id = NULL;
 void * pmemPtr = (void *) eglImage;
    temp_egl_id = (struct temp_egl *)eglImage;
 if (temp_egl_id != NULL) {
        fd = temp_egl_id->pmem_fd;
        offset = temp_egl_id->offset;
 }
#endif
 if (fd < 0) {
        DEBUG_PRINT_ERROR("Improper pmem fd by EGL client %d",fd);
 return OMX_ErrorInsufficientResources;
 }
    pmem_info.pmem_fd = (OMX_U32) fd;
    pmem_info.offset = (OMX_U32) offset;
    pmem_entry.entry = (void *) &pmem_info;
    pmem_entry.type = OMX_QCOM_PLATFORM_PRIVATE_PMEM;
    pmem_list.entryList = &pmem_entry;
    pmem_list.nEntries = 1;
    ouput_egl_buffers = true;
 if (OMX_ErrorNone != use_buffer(hComp,bufferHdr, port,
 (void *)&pmem_list, drv_ctx.op_buf.buffer_size,
 (OMX_U8 *)pmemPtr)) {
        DEBUG_PRINT_ERROR("use buffer call failed for egl image");
 return OMX_ErrorInsufficientResources;
 }
 return OMX_ErrorNone;
}
