OMX_ERRORTYPE  omx_vdec::get_state(OMX_IN OMX_HANDLETYPE  hComp,
        OMX_OUT OMX_STATETYPE* state)
{
 (void) hComp;
 *state = m_state;
    DEBUG_PRINT_LOW("get_state: Returning the state %d",*state);
 return OMX_ErrorNone;
}
