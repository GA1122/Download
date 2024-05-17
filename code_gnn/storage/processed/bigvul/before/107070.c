void QQuickWebViewPrivate::setNavigatorQtObjectEnabled(bool enabled)
{
    ASSERT(enabled != m_navigatorQtObjectEnabled);
    m_navigatorQtObjectEnabled = enabled;
    context->setNavigatorQtObjectEnabled(webPageProxy.get(), enabled);
}
