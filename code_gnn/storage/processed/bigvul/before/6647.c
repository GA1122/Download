SchedulerObject::SchedulerObject()
{
    m_pool = getPoolName();
	m_name = getScheddName();
    m_codec = new BaseCodec();
}
