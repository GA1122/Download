void SVGDocumentExtensions::addSVGRootWithRelativeLengthDescendents(SVGSVGElement* svgRoot)
{
    ASSERT(!m_inRelativeLengthSVGRootsInvalidation);
    m_relativeLengthSVGRoots.add(svgRoot);
}
