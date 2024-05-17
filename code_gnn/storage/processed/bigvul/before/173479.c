bool omx_vdec::post_event(unsigned long p1,
 unsigned long p2,
 unsigned long id)
{
 bool bRet = false;

  
 if (m_error_propogated) {
 switch (id) {
 case OMX_COMPONENT_GENERATE_PORT_RECONFIG:
 case OMX_COMPONENT_GENERATE_HARDWARE_ERROR:
                DEBUG_PRINT_ERROR("Dropping message %lx "
 "since client expected to be in error state", id);
 return false;
 default:
  
 break;
 }
 }

    pthread_mutex_lock(&m_lock);

 if (id == m_fill_output_msg ||
            id == OMX_COMPONENT_GENERATE_FBD ||
            id == OMX_COMPONENT_GENERATE_PORT_RECONFIG) {
        m_ftb_q.insert_entry(p1,p2,id);
 } else if (id == OMX_COMPONENT_GENERATE_ETB ||
            id == OMX_COMPONENT_GENERATE_EBD ||
            id == OMX_COMPONENT_GENERATE_ETB_ARBITRARY) {
        m_etb_q.insert_entry(p1,p2,id);
 } else {
        m_cmd_q.insert_entry(p1,p2,id);
 }

    bRet = true;
    DEBUG_PRINT_LOW("Value of this pointer in post_event %p",this);
    post_message(this, id);

    pthread_mutex_unlock(&m_lock);

 return bRet;
}
