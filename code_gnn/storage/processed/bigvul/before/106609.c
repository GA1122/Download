void WebPageProxy::saveDataToFileInDownloadsFolder(const String& suggestedFilename, const String& mimeType, const String& originatingURLString, WebData* data)
{
    m_uiClient.saveDataToFileInDownloadsFolder(this, suggestedFilename, mimeType, originatingURLString, data);
}
