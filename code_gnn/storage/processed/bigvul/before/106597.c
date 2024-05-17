void WebPageProxy::recordAutocorrectionResponse(int32_t responseType, const String& replacedString, const String& replacementString)
{
    m_pageClient->recordAutocorrectionResponse((WebCore::EditorClient::AutocorrectionResponseType)responseType, replacedString, replacementString);
}
