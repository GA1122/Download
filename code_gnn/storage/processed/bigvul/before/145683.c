void ImageInputType::createShadowSubtree()
{
    if (!m_useFallbackContent) {
        BaseButtonInputType::createShadowSubtree();
        return;
    }
    HTMLImageFallbackHelper::createAltTextShadowTree(element());
}
