KURL Document::completeURLWithOverride(const String& url, const KURL& baseURLOverride) const
{
    ASSERT(baseURLOverride.isEmpty() || baseURLOverride.isValid());

    if (url.isNull())
        return KURL();
    const KURL* baseURLFromParent = 0;
    bool shouldUseParentBaseURL = baseURLOverride.isEmpty();
    if (!shouldUseParentBaseURL) {
        const KURL& aboutBlankURL = blankURL();
        shouldUseParentBaseURL = (baseURLOverride == aboutBlankURL);
    }
    if (shouldUseParentBaseURL) {
        if (Document* parent = parentDocument())
            baseURLFromParent = &parent->baseURL();
    }
    const KURL& baseURL = baseURLFromParent ? *baseURLFromParent : baseURLOverride;
    if (!encoding().isValid())
        return KURL(baseURL, url);
    return KURL(baseURL, url, encoding());
}
