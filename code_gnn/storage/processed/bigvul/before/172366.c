bool omx_video::post_event(unsigned long p1,
 unsigned long p2,
 unsigned long id)
{
 bool bRet = false;

    pthread_mutex_lock(&m_lock);

 if ((id == OMX_COMPONENT_GENERATE_FTB) ||
 (id == OMX_COMPONENT_GENERATE_FBD) ||
 (id == OMX_COMPONENT_GENERATE_EVENT_OUTPUT_FLUSH)) {
        m_ftb_q.insert_entry(p1,p2,id);
 } else if ((id == OMX_COMPONENT_GENERATE_ETB) ||
 (id == OMX_COMPONENT_GENERATE_EBD) ||
 (id == OMX_COMPONENT_GENERATE_EVENT_INPUT_FLUSH)) {
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
