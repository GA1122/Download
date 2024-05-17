void RTCVoidRequestImpl::requestFailed(const String& error)
{
    if (m_errorCallback.get())
        m_errorCallback->handleEvent(error);

    clear();
}
