void WebPageProxy::didChangeViewportData(const ViewportArguments& args)
{
    m_pageClient->setViewportArguments(args);
}
