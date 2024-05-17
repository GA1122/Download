void SVGDocumentExtensions::removeResource(const AtomicString& id)
{
    if (id.isEmpty())
        return;

    m_resources.remove(id);
}
