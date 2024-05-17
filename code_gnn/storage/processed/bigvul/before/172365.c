bool omx_video::omx_cmd_queue::pop_entry(unsigned long *p1, unsigned long *p2, unsigned long *id)
{
 bool ret = true;
 if (m_size > 0) {
 *id = m_q[m_read].id;
 *p1 = m_q[m_read].param1;
 *p2 = m_q[m_read].param2;
 ++m_read;
 --m_size;
 if (m_read >= OMX_CORE_CONTROL_CMDQ_SIZE) {
            m_read = 0;
 }
 } else {
        ret = false;
 }
 return ret;
}
