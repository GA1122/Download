static bool isEligibleForSeamless(Document* parent, Document* child)
{
    if (!parent)
        return false;
    if (parent->isSandboxed(SandboxSeamlessIframes))
        return false;
    if (child->isSrcdocDocument())
        return true;
    if (parent->securityOrigin()->canAccess(child->securityOrigin()))
        return true;
    return parent->securityOrigin()->canRequest(child->url());
}
