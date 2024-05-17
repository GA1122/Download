WebRTCVoidRequest::ExtraData* WebRTCVoidRequest::extraData() const
{
    RefPtr<RTCVoidRequest::ExtraData> data = m_private->extraData();
    if (!data)
        return 0;
    return static_cast<ExtraDataContainer*>(data.get())->extraData();
}
