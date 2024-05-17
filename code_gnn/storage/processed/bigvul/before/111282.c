int WebPagePrivate::playerID() const
{
    return m_client ? m_client->getInstanceId() : 0;
}
