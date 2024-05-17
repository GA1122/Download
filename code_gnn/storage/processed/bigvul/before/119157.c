 bool XMLHttpRequest::canSuspend() const
{
    return !m_loader;
}
