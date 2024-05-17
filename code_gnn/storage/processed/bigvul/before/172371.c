OMX_ERRORTYPE omx_video::queue_meta_buffer(OMX_HANDLETYPE hComp,
 struct pmem &Input_pmem_info)
{

    OMX_ERRORTYPE ret = OMX_ErrorNone;
 unsigned long address = 0,p2,id;

    DEBUG_PRINT_LOW("In queue Meta Buffer");
 if (!psource_frame || !pdest_frame) {
        DEBUG_PRINT_ERROR("convert_queue_buffer invalid params");
 return OMX_ErrorBadParameter;
 }

 if (psource_frame->nFilledLen > 0) {
 if (dev_use_buf(&Input_pmem_info,PORT_INDEX_IN,0) != true) {
            DEBUG_PRINT_ERROR("ERROR: in dev_use_buf");
            post_event ((unsigned long)psource_frame,0,OMX_COMPONENT_GENERATE_EBD);
            ret = OMX_ErrorBadParameter;
 }
 }

 if (ret == OMX_ErrorNone)
        ret = empty_this_buffer_proxy(hComp,psource_frame);

 if (ret == OMX_ErrorNone) {
        psource_frame = NULL;
 if (!psource_frame && m_opq_meta_q.m_size) {
            m_opq_meta_q.pop_entry(&address,&p2,&id);
            psource_frame = (OMX_BUFFERHEADERTYPE* ) address;
 }
 } else {
        psource_frame = NULL;
 }
 return ret;
}
