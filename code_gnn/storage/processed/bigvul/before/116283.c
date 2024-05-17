QUrl QQuickWebViewExperimental::remoteInspectorUrl() const
{
    return QUrl(WebInspectorServer::shared().inspectorUrlForPageID(d_ptr->webPageProxy->inspector()->remoteInspectionPageID()));
}
