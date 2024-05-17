void Document::close(ExceptionState& exceptionState)
{

    if (importLoader()) {
        exceptionState.throwDOMException(InvalidStateError, "Imported document doesn't support close().");
        return;
    }

    if (!isHTMLDocument()) {
        exceptionState.throwDOMException(InvalidStateError, "Only HTML documents support close().");
        return;
    }

    close();
}
