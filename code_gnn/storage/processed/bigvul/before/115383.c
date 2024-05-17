uint64_t InjectedBundlePage::didExceedDatabaseQuota(WKSecurityOriginRef origin, WKStringRef databaseName, WKStringRef databaseDisplayName, uint64_t currentQuotaBytes, uint64_t currentOriginUsageBytes, uint64_t currentDatabaseUsageBytes, uint64_t expectedUsageBytes)
{
    if (InjectedBundle::shared().testRunner()->shouldDumpDatabaseCallbacks()) {
        StringBuilder stringBuilder;
        stringBuilder.appendLiteral("UI DELEGATE DATABASE CALLBACK: exceededDatabaseQuotaForSecurityOrigin:");
        stringBuilder.append(securityOriginToStr(origin));
        stringBuilder.appendLiteral(" database:");
        stringBuilder.append(toWTFString(databaseName));
        stringBuilder.append('\n');
        InjectedBundle::shared().outputText(stringBuilder.toString());
    }

    static const uint64_t defaultQuota = 5 * 1024 * 1024;
    return defaultQuota;
}
