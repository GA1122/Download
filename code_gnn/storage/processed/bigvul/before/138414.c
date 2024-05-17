void Document::didLoadAllScriptBlockingResources()
{
    m_executeScriptsWaitingForResourcesTimer.startOneShot(0, FROM_HERE);

    if (frame())
        frame()->loader().client()->didRemoveAllPendingStylesheet();

    if (m_gotoAnchorNeededAfterStylesheetsLoad && view())
        view()->scrollToFragment(m_url);
}
