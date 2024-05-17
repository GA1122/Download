static void handleMetaViewport(const String& attributeValue, const CachedDocumentParameters* documentParameters, MediaValuesCached* mediaValues, ViewportDescriptionWrapper* viewport)
{
    if (!documentParameters->viewportMetaEnabled)
        return;
    ViewportDescription description(ViewportDescription::ViewportMeta);
    HTMLMetaElement::getViewportDescriptionFromContentAttribute(attributeValue, description, nullptr, documentParameters->viewportMetaZeroValuesQuirk);
    if (viewport) {
        viewport->description = description;
        viewport->set = true;
    }
    FloatSize initialViewport(mediaValues->deviceWidth(), mediaValues->deviceHeight());
    PageScaleConstraints constraints = description.resolve(initialViewport, documentParameters->defaultViewportMinWidth);
    mediaValues->overrideViewportDimensions(constraints.layoutSize.width(), constraints.layoutSize.height());
}
