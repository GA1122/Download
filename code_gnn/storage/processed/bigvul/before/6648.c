SchedulerObject* SchedulerObject::getInstance()
{
    if (!m_instance) {
        m_instance = new SchedulerObject();
    }
    return m_instance;
}
