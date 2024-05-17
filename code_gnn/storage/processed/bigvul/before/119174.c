String XMLHttpRequest::getResponseHeader(const AtomicString& name, ExceptionState& es) const
{
    if (m_state < HEADERS_RECEIVED) {
        es.throwDOMException(InvalidStateError, ExceptionMessages::failedToExecute("getResponseHeader", "XMLHttpRequest", "the object's state must not be UNSENT or OPENED."));
        return String();
    }

    if (isSetCookieHeader(name) && !securityOrigin()->canLoadLocalResources()) {
        logConsoleError(scriptExecutionContext(), "Refused to get unsafe header \"" + name + "\"");
        return String();
    }

    HTTPHeaderSet accessControlExposeHeaderSet;
    parseAccessControlExposeHeadersAllowList(m_response.httpHeaderField("Access-Control-Expose-Headers"), accessControlExposeHeaderSet);

    if (!m_sameOriginRequest && !isOnAccessControlResponseHeaderWhitelist(name) && !accessControlExposeHeaderSet.contains(name)) {
        logConsoleError(scriptExecutionContext(), "Refused to get unsafe header \"" + name + "\"");
        return String();
    }
    return m_response.httpHeaderField(name);
}
