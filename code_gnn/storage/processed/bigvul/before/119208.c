void XMLHttpRequest::sendForInspectorXHRReplay(PassRefPtr<FormData> formData, ExceptionState& es)
{
    m_requestEntityBody = formData ? formData->deepCopy() : 0;
    createRequest(es);
    m_exceptionCode = es.code();
}
