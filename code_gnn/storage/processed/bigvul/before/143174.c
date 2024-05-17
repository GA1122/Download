void Document::didLoadAllScriptBlockingResources()
{
    loadingTaskRunner()->postTask(BLINK_FROM_HERE, m_executeScriptsWaitingForResourcesTask->cancelAndCreate());

    if (isHTMLDocument() && body()) {
        beginLifecycleUpdatesIfRenderingReady();
    } else if (!isHTMLDocument() && documentElement()) {
        beginLifecycleUpdatesIfRenderingReady();
    }

    if (m_gotoAnchorNeededAfterStylesheetsLoad && view())
        view()->processUrlFragment(m_url);
}
