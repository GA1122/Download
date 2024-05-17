void TextTrackLoader::corsPolicyPreventedLoad(SecurityOrigin* securityOrigin, const KURL& url)
{
    String consoleMessage("Text track from origin '" + SecurityOrigin::create(url)->toString() + "' has been blocked from loading: Not at same origin as the document, and parent of track element does not have a 'crossorigin' attribute. Origin '" + securityOrigin->toString() + "' is therefore not allowed access.");
    document().addConsoleMessage(ConsoleMessage::create(SecurityMessageSource, ErrorMessageLevel, consoleMessage));
    m_state = Failed;
}
