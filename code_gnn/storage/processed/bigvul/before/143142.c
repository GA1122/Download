KURL Document::completeURLWithOverride(const String& url, const KURL& baseURLOverride) const
{
    DCHECK(baseURLOverride.isEmpty() || baseURLOverride.isValid());

    if (url.isNull())
        return KURL();
    const KURL& baseURL = baseURLForOverride(baseURLOverride);
    if (!encoding().isValid())
        return KURL(baseURL, url);
    return KURL(baseURL, url, encoding());
}
