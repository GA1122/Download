JSObject* addErrorInfo(CallFrame* callFrame, JSObject* error, int line, const SourceCode& source)
{
    JSGlobalData* globalData = &callFrame->globalData();
    const UString& sourceURL = source.provider()->url();

    if (line != -1)
        error->putDirect(*globalData, Identifier(globalData, linePropertyName), jsNumber(line), ReadOnly | DontDelete);
    if (!sourceURL.isNull())
        error->putDirect(*globalData, Identifier(globalData, sourceURLPropertyName), jsString(globalData, sourceURL), ReadOnly | DontDelete);

    globalData->interpreter->addStackTraceIfNecessary(callFrame, error);

    return error;
}
