XMLHttpRequest::XMLHttpRequest(ScriptExecutionContext* context, PassRefPtr<SecurityOrigin> securityOrigin)
    : ActiveDOMObject(context)
    , m_async(true)
    , m_includeCredentials(false)
    , m_timeoutMilliseconds(0)
    , m_state(UNSENT)
    , m_createdDocument(false)
    , m_error(false)
    , m_uploadEventsAllowed(true)
    , m_uploadComplete(false)
    , m_sameOriginRequest(true)
    , m_receivedLength(0)
    , m_lastSendLineNumber(0)
    , m_exceptionCode(0)
    , m_progressEventThrottle(this)
    , m_responseTypeCode(ResponseTypeDefault)
    , m_protectionTimer(this, &XMLHttpRequest::dropProtection)
    , m_securityOrigin(securityOrigin)
{
    initializeXMLHttpRequestStaticData();
#ifndef NDEBUG
    xmlHttpRequestCounter.increment();
#endif
    ScriptWrappable::init(this);
}
