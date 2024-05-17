bool SVGDocumentExtensions::isSVGRootWithRelativeLengthDescendents(SVGSVGElement* svgRoot) const
{
    return m_relativeLengthSVGRoots.contains(svgRoot);
}
