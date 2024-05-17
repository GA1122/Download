static void appendImageIfNotNull(Vector<ImageResource*>& imageResources, const StyleImage* styleImage)
{
    if (styleImage && styleImage->cachedImage()) {
        ImageResource* imageResource = styleImage->cachedImage();
        if (imageResource && !imageResource->isLoaded())
            imageResources.append(styleImage->cachedImage());
    }
}
