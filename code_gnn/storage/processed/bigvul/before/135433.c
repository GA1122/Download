void Document::updateStyleInvalidationIfNeeded()
{
    ScriptForbiddenScope forbidScript;

    if (!isActive())
        return;
    if (!childNeedsStyleInvalidation())
        return;
    TRACE_EVENT0("blink", "Document::updateStyleInvalidationIfNeeded");
    styleEngine().styleInvalidator().invalidate(*this);
}
