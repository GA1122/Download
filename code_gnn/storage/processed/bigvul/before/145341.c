void HTMLAnchorElement::parseAttribute(const QualifiedName& name, const AtomicString& oldValue, const AtomicString& value)
{
    if (name == hrefAttr) {
        bool wasLink = isLink();
        setIsLink(!value.isNull());
        if (wasLink || isLink()) {
            pseudoStateChanged(CSSSelector::PseudoLink);
            pseudoStateChanged(CSSSelector::PseudoVisited);
            pseudoStateChanged(CSSSelector::PseudoAnyLink);
        }
        if (wasLink && !isLink() && treeScope().adjustedFocusedElement() == this) {
            document().setNeedsFocusedElementCheck();
        }
        if (isLink()) {
            String parsedURL = stripLeadingAndTrailingHTMLSpaces(value);
            if (document().isDNSPrefetchEnabled()) {
                if (protocolIs(parsedURL, "http") || protocolIs(parsedURL, "https") || parsedURL.startsWith("//"))
                    prefetchDNS(document().completeURL(parsedURL).host());
            }
        }
        invalidateCachedVisitedLinkHash();
        logUpdateAttributeIfIsolatedWorldAndInDocument("a", hrefAttr, oldValue, value);
    } else if (name == nameAttr || name == titleAttr) {
    } else if (name == relAttr) {
        setRel(value);
    } else {
        HTMLElement::parseAttribute(name, oldValue, value);
    }
}
