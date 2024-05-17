void Document::open(Document* enteredDocument, ExceptionState& exceptionState)
{
    if (importLoader()) {
        exceptionState.throwDOMException(InvalidStateError, "Imported document doesn't support open().");
        return;
    }

    if (!isHTMLDocument()) {
        exceptionState.throwDOMException(InvalidStateError, "Only HTML documents support open().");
        return;
    }

    if (enteredDocument) {
        if (!securityOrigin()->canAccess(enteredDocument->securityOrigin())) {
            exceptionState.throwSecurityError("Can only call open() on same-origin documents.");
            return;
        }
        setSecurityOrigin(enteredDocument->securityOrigin());
        setURL(enteredDocument->url());
        m_cookieURL = enteredDocument->cookieURL();
    }

    open();
}
