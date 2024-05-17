void Document::enforceSandboxFlags(SandboxFlags mask)
{
    RefPtr<SecurityOrigin> standInOrigin = getSecurityOrigin();
    applySandboxFlags(mask);
    if (standInOrigin && !standInOrigin->isUnique() && getSecurityOrigin()->isUnique()) {
        getSecurityOrigin()->setUniqueOriginIsPotentiallyTrustworthy(standInOrigin->isPotentiallyTrustworthy());
        if (frame())
            frame()->loader().client()->didUpdateToUniqueOrigin();
    }
}
