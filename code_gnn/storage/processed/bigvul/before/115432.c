static inline void dumpResourceURL(uint64_t identifier, StringBuilder& stringBuilder)
{
    if (assignedUrlsCache.contains(identifier))
        stringBuilder.append(assignedUrlsCache.get(identifier));
    else
        stringBuilder.appendLiteral("<unknown>");
}
