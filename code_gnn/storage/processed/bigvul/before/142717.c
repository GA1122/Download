bool FrameLoader::shouldPerformFragmentNavigation(bool isFormSubmission, const String& httpMethod, FrameLoadType loadType, const KURL& url)
{
    return (!isFormSubmission || equalIgnoringCase(httpMethod, HTTPNames::GET))
        && loadType != FrameLoadTypeReload
        && loadType != FrameLoadTypeReloadBypassingCache
        && loadType != FrameLoadTypeReloadMainResource
        && loadType != FrameLoadTypeBackForward
        && url.hasFragmentIdentifier()
        && equalIgnoringFragmentIdentifier(m_frame->document()->url(), url)
        && !m_frame->document()->isFrameSet();
}
