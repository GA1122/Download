static bool shouldSendFinishNotification(LocalFrame* frame)
{
    if (!frame->loader().stateMachine()->committedFirstRealDocumentLoad())
        return false;

    if (frame->loader().documentLoader()->sentDidFinishLoad())
        return false;

    if (!frame->document()->loadEventFinished())
        return false;

    if (!allDescendantsAreComplete(frame))
        return false;
    return true;
}
