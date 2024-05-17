OMX_ERRORTYPE  omx_vdec::component_tunnel_request(OMX_IN OMX_HANDLETYPE  hComp,
        OMX_IN OMX_U32                        port,
        OMX_IN OMX_HANDLETYPE        peerComponent,
        OMX_IN OMX_U32                    peerPort,
        OMX_INOUT OMX_TUNNELSETUPTYPE* tunnelSetup)
{
 (void) hComp;
 (void) port;
 (void) peerComponent;
 (void) peerPort;
 (void) tunnelSetup;
    DEBUG_PRINT_ERROR("Error: component_tunnel_request Not Implemented");
 return OMX_ErrorNotImplemented;
}
