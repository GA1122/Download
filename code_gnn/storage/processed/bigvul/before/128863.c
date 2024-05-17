void SVGDocumentExtensions::removeSVGRootWithRelativeLengthDescendents(SVGSVGElement* svgRoot)
{
    ASSERT(!m_inRelativeLengthSVGRootsInvalidation);
    m_relativeLengthSVGRoots.remove(svgRoot);
}
