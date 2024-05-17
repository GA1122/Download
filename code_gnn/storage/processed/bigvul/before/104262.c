void RTCVoidRequestImpl::requestSucceeded()
{
    if (m_successCallback)
        m_successCallback->handleEvent();

    clear();
}
