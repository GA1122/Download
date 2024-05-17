bool DocumentLoader::maybeLoadEmpty()
{
    bool shouldLoadEmpty = !m_substituteData.isValid() && (m_request.url().isEmpty() || SchemeRegistry::shouldLoadURLSchemeAsEmptyDocument(m_request.url().protocol()));
    if (!shouldLoadEmpty && !frameLoader()->client()->representationExistsForURLScheme(m_request.url().protocol()))
        return false;

    if (m_request.url().isEmpty() && !frameLoader()->stateMachine()->creatingInitialEmptyDocument())
        m_request.setURL(blankURL());
    String mimeType = shouldLoadEmpty ? "text/html" : frameLoader()->client()->generatedMIMETypeForURLScheme(m_request.url().protocol());
    m_response = ResourceResponse(m_request.url(), mimeType, 0, String(), String());
    finishedLoading(monotonicallyIncreasingTime());
    return true;
}
