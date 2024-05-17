void HTMLConstructionSite::generateImpliedEndTagsWithExclusion(const AtomicString& tagName)
{
    while (hasImpliedEndTag(currentElement()) && !currentElement()->hasLocalName(tagName))
        m_openElements.pop();
}
