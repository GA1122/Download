void SVGDocumentExtensions::invalidateSVGRootsWithRelativeLengthDescendents(SubtreeLayoutScope* scope)
{
    ASSERT(!m_inRelativeLengthSVGRootsInvalidation);
#if !ASSERT_DISABLED
    TemporaryChange<bool> inRelativeLengthSVGRootsChange(m_inRelativeLengthSVGRootsInvalidation, true);
#endif

    HashSet<SVGSVGElement*>::iterator end = m_relativeLengthSVGRoots.end();
    for (HashSet<SVGSVGElement*>::iterator it = m_relativeLengthSVGRoots.begin(); it != end; ++it)
        (*it)->invalidateRelativeLengthClients(scope);
}
