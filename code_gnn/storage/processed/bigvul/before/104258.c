RTCVoidRequestImpl::RTCVoidRequestImpl(ScriptExecutionContext* context, PassRefPtr<VoidCallback> successCallback, PassRefPtr<RTCErrorCallback> errorCallback)
    : ActiveDOMObject(context, this)
    , m_successCallback(successCallback)
    , m_errorCallback(errorCallback)
{
}
