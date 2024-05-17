bool WebPageProxy::isValid()
{
    if (m_isClosed)
        return false;

    return m_isValid;
}
