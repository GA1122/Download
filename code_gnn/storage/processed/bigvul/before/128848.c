bool SVGDocumentExtensions::isElementPendingResources(Element* element) const
{

    ASSERT(element);

    HashMap<AtomicString, OwnPtr<SVGPendingElements> >::const_iterator end = m_pendingResources.end();
    for (HashMap<AtomicString, OwnPtr<SVGPendingElements> >::const_iterator it = m_pendingResources.begin(); it != end; ++it) {
        SVGPendingElements* elements = it->value.get();
        ASSERT(elements);

        if (elements->contains(element))
            return true;
    }
    return false;
}
