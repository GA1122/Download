void SVGDocumentExtensions::registerPendingSVGFontFaceElementsForRemoval(PassRefPtrWillBeRawPtr<SVGFontFaceElement> font)
{
    m_pendingSVGFontFaceElementsForRemoval.add(font);
}
