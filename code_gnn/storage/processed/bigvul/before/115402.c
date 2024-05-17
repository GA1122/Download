void InjectedBundlePage::didReachApplicationCacheOriginQuota(WKSecurityOriginRef origin, int64_t totalBytesNeeded)
{
    if (InjectedBundle::shared().testRunner()->shouldDumpApplicationCacheDelegateCallbacks()) {
        int64_t truncatedSpaceNeeded = (totalBytesNeeded / 10000) * 10000;

        StringBuilder stringBuilder;
        stringBuilder.appendLiteral("UI DELEGATE APPLICATION CACHE CALLBACK: exceededApplicationCacheOriginQuotaForSecurityOrigin:");
        stringBuilder.append(securityOriginToStr(origin));
        stringBuilder.appendLiteral(" totalSpaceNeeded:~");
        stringBuilder.appendNumber(truncatedSpaceNeeded);
        stringBuilder.append('\n');
        InjectedBundle::shared().outputText(stringBuilder.toString());
    }

    if (InjectedBundle::shared().testRunner()->shouldDisallowIncreaseForApplicationCacheQuota())
        return;

    WKBundleResetApplicationCacheOriginQuota(InjectedBundle::shared().bundle(), adoptWK(WKSecurityOriginCopyToString(origin)).get());
}
