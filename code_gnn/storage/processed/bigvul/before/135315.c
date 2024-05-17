KURL Document::openSearchDescriptionURL()
{
    static const char openSearchMIMEType[] = "application/opensearchdescription+xml";
    static const char openSearchRelation[] = "search";

    if (!frame() || frame()->tree().parent())
        return KURL();

    if (!loadEventFinished())
        return KURL();

    if (!head())
        return KURL();

    for (HTMLLinkElement* linkElement = Traversal<HTMLLinkElement>::firstChild(*head()); linkElement; linkElement = Traversal<HTMLLinkElement>::nextSibling(*linkElement)) {
        if (!equalIgnoringCase(linkElement->type(), openSearchMIMEType) || !equalIgnoringCase(linkElement->rel(), openSearchRelation))
            continue;
        if (linkElement->href().isEmpty())
            continue;
        return linkElement->href();
    }

    return KURL();
}
