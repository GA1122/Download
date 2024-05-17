void WebPagePrivate::stopCurrentLoad()
{
    m_mainFrame->loader()->stopAllLoaders();

    DeferredTaskLoadManualScript::finishOrCancel(this);
}
