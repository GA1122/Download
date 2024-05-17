void Document::didLoadAllScriptBlockingResources()
{
    Platform::current()->currentThread()->scheduler()->loadingTaskRunner()->postTask(
        FROM_HERE, m_executeScriptsWaitingForResourcesTask->cancelAndCreate());

    if (frame())
        frame()->loader().client()->didRemoveAllPendingStylesheet();

    if (m_gotoAnchorNeededAfterStylesheetsLoad && view())
        view()->processUrlFragment(m_url);
}
