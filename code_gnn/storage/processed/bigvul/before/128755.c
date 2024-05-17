bool ReadableStream::enqueuePreliminaryCheck()
{
    if (m_state == Errored || m_state == Closed || m_isDraining)
        return false;

    return true;
}
