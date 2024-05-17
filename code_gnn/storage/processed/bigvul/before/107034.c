void QQuickWebViewPrivate::onComponentComplete()
{
    if (m_deferedUrlToLoad.isEmpty())
        return;

    q_ptr->setUrl(m_deferedUrlToLoad);
}
