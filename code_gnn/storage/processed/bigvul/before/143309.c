static bool canAccessAncestor(const SecurityOrigin& activeSecurityOrigin, const Frame* targetFrame)
{
    if (!targetFrame)
        return false;

    const bool isLocalActiveOrigin = activeSecurityOrigin.isLocal();
    for (const Frame* ancestorFrame = targetFrame; ancestorFrame; ancestorFrame = ancestorFrame->tree().parent()) {
        const SecurityOrigin* ancestorSecurityOrigin = ancestorFrame->securityContext()->getSecurityOrigin();
        if (activeSecurityOrigin.canAccess(ancestorSecurityOrigin))
            return true;

        if (isLocalActiveOrigin && ancestorSecurityOrigin->isLocal())
            return true;
    }

    return false;
}
