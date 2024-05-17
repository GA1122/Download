static void appendImagesFromStyle(Vector<ImageResource*>& images, RenderStyle& blockStyle)
{
    appendLayers(images, blockStyle.backgroundLayers());
    appendLayers(images, blockStyle.maskLayers());

    const ContentData* contentData = blockStyle.contentData();
    if (contentData && contentData->isImage()) {
        const ImageContentData* imageContentData = static_cast<const ImageContentData*>(contentData);
        appendImageIfNotNull(images, imageContentData->image());
    }
    if (blockStyle.boxReflect())
        appendImageIfNotNull(images, blockStyle.boxReflect()->mask().image());
    appendImageIfNotNull(images, blockStyle.listStyleImage());
    appendImageIfNotNull(images, blockStyle.borderImageSource());
    appendImageIfNotNull(images, blockStyle.maskBoxImageSource());
    if (blockStyle.shapeOutside())
        appendImageIfNotNull(images, blockStyle.shapeOutside()->image());
}
