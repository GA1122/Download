void Document::write(const SegmentedString& text, Document* enteredDocument, ExceptionState& exceptionState)
{
    if (importLoader()) {
        exceptionState.throwDOMException(InvalidStateError, "Imported document doesn't support write().");
        return;
    }

    if (!isHTMLDocument()) {
        exceptionState.throwDOMException(InvalidStateError, "Only HTML documents support write().");
        return;
    }

    if (enteredDocument && !securityOrigin()->canAccess(enteredDocument->securityOrigin())) {
        exceptionState.throwSecurityError("Can only call write() on same-origin documents.");
        return;
    }

    NestingLevelIncrementer nestingLevelIncrementer(m_writeRecursionDepth);

    m_writeRecursionIsTooDeep = (m_writeRecursionDepth > 1) && m_writeRecursionIsTooDeep;
    m_writeRecursionIsTooDeep = (m_writeRecursionDepth > cMaxWriteRecursionDepth) || m_writeRecursionIsTooDeep;

    if (m_writeRecursionIsTooDeep)
        return;

    bool hasInsertionPoint = m_parser && m_parser->hasInsertionPoint();

    if (!hasInsertionPoint && m_ignoreDestructiveWriteCount) {
        addConsoleMessage(ConsoleMessage::create(JSMessageSource, WarningMessageLevel, ExceptionMessages::failedToExecute("write", "Document", "It isn't possible to write into a document from an asynchronously-loaded external script unless it is explicitly opened.")));
        return;
    }

    if (!hasInsertionPoint)
        open(enteredDocument, ASSERT_NO_EXCEPTION);

    ASSERT(m_parser);
    m_parser->insert(text);
}
