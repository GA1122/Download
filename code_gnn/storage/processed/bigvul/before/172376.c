OMX_ERRORTYPE  omx_video::set_callbacks(OMX_IN OMX_HANDLETYPE        hComp,
        OMX_IN OMX_CALLBACKTYPE* callbacks,
        OMX_IN OMX_PTR             appData)
{
 (void)hComp;
    m_pCallbacks       = *callbacks;
    DEBUG_PRINT_LOW("Callbacks Set %p %p %p",m_pCallbacks.EmptyBufferDone,\
            m_pCallbacks.EventHandler,m_pCallbacks.FillBufferDone);
    m_app_data =    appData;
 return OMX_ErrorNotImplemented;
}
