static bool shouldSendCompleteNotification(LocalFrame* frame)
{
    if (!frame->isLoading())
        return false;
    return frame->loader().documentLoader()->sentDidFinishLoad() && !frame->loader().provisionalDocumentLoader() && !frame->loader().client()->hasPendingNavigation();
}
