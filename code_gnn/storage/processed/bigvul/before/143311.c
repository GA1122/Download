bool Frame::canNavigateWithoutFramebusting(const Frame& targetFrame, String& reason)
{
    if (securityContext()->isSandboxed(SandboxNavigation)) {
        if (targetFrame.tree().isDescendantOf(this))
            return true;

        if (targetFrame == targetFrame.tree().top() && targetFrame.tree().top() != tree().top() && !securityContext()->isSandboxed(SandboxPropagatesToAuxiliaryBrowsingContexts))
            return true;

        if (securityContext()->isSandboxed(SandboxTopNavigation) && targetFrame == tree().top())
            reason = "The frame attempting navigation of the top-level window is sandboxed, but the 'allow-top-navigation' flag is not set.";
        else
            reason = "The frame attempting navigation is sandboxed, and is therefore disallowed from navigating its ancestors.";
        return false;
    }

    ASSERT(securityContext()->getSecurityOrigin());
    SecurityOrigin& origin = *securityContext()->getSecurityOrigin();

    if (canAccessAncestor(origin, &targetFrame))
        return true;

    if (!targetFrame.tree().parent()) {
        if (targetFrame == client()->opener())
            return true;
        if (canAccessAncestor(origin, targetFrame.client()->opener()))
            return true;
    }

    reason = "The frame attempting navigation is neither same-origin with the target, nor is it the target's parent or opener.";
    return false;
}
