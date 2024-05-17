void SVGDocumentExtensions::trace(Visitor* visitor)
{
    visitor->trace(m_timeContainers);
    visitor->trace(m_svgFontFaceElements);
    visitor->trace(m_pendingSVGFontFaceElementsForRemoval);
}
