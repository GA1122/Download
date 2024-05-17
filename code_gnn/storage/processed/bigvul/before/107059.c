void QQuickWebViewExperimental::sendApplicationSchemeReply(QQuickNetworkReply* reply)
{
    d_ptr->webPageProxy->sendApplicationSchemeReply(reply);
}
