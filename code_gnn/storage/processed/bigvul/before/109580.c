bool Document::canNavigate(Frame* targetFrame)
{
    if (!m_frame)
        return false;

    if (!targetFrame)
        return true;

    if (!isSandboxed(SandboxTopNavigation) && targetFrame == m_frame->tree()->top())
        return true;

    if (isSandboxed(SandboxNavigation)) {
        if (targetFrame->tree()->isDescendantOf(m_frame))
            return true;

        const char* reason = "The frame attempting navigation is sandboxed, and is therefore disallowed from navigating its ancestors.";
        if (isSandboxed(SandboxTopNavigation) && targetFrame == m_frame->tree()->top())
            reason = "The frame attempting navigation of the top-level window is sandboxed, but the 'allow-top-navigation' flag is not set.";

        printNavigationErrorMessage(targetFrame, url(), reason);
        return false;
    }

    if (canAccessAncestor(securityOrigin(), targetFrame))
        return true;

    if (!targetFrame->tree()->parent()) {
        if (targetFrame == m_frame->loader()->opener())
            return true;

        if (canAccessAncestor(securityOrigin(), targetFrame->loader()->opener()))
            return true;
    }

    printNavigationErrorMessage(targetFrame, url(), "The frame attempting navigation is neither same-origin with the target, nor is it the target's parent or opener.");
    return false;
}
