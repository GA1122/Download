PassRefPtr<ComputedStyle> ImageInputType::customStyleForLayoutObject(PassRefPtr<ComputedStyle> newStyle)
{
    if (!m_useFallbackContent)
        return newStyle;

    return HTMLImageFallbackHelper::customStyleForAltText(element(), newStyle);
}
