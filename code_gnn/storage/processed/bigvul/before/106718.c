void WebView::initializeUndoClient(const WKViewUndoClient* client)
{
    m_undoClient.initialize(client);
}
