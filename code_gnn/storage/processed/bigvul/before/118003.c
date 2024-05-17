void V8Proxy::reportUnsafeAccessTo(Frame* target)
{
    ASSERT(target);
    Document* targetDocument = target->document();
    if (!targetDocument)
        return;

    Frame* source = V8Proxy::retrieveFrameForEnteredContext();
    if (!source)
        return;

    Document* sourceDocument = source->document();
    if (!sourceDocument)
        return;  

    String str = "Unsafe JavaScript attempt to access frame with URL " + targetDocument->url().string() +
                 " from frame with URL " + sourceDocument->url().string() + ". Domains, protocols and ports must match.\n";

    RefPtr<ScriptCallStack> stackTrace = createScriptCallStack(ScriptCallStack::maxCallStackSizeToCapture, true);

    sourceDocument->addConsoleMessage(JSMessageSource, LogMessageType, ErrorMessageLevel, str, stackTrace.release());
}
