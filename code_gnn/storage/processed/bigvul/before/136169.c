void Document::open(Document* ownerDocument, ExceptionState& exceptionState)
{
    if (importLoader()) {
        exceptionState.throwDOMException(InvalidStateError, "Imported document doesn't support open().");
        return;
    }

    if (!isHTMLDocument()) {
        exceptionState.throwDOMException(InvalidStateError, "Only HTML documents support open().");
        return;
    }

    if (ownerDocument) {
        setURL(ownerDocument->url());
        m_cookieURL = ownerDocument->cookieURL();
        setSecurityOrigin(ownerDocument->securityOrigin());
    }

    open();
}
