bool HTMLScriptRunner::requestPendingScript(PendingScript& pendingScript, Element* script) const
{
    ASSERT(!pendingScript.element());
    pendingScript.setElement(script);
    ScriptResource* resource = toScriptLoaderIfPossible(script)->resource().get();
    if (!resource) {
        notImplemented();  
        return false;
    }
    pendingScript.setScriptResource(resource);
    return true;
}
