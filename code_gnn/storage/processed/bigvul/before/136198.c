void PageSerializer::addImageToResources(ImageResource* image, LayoutObject* imageLayoutObject, const KURL& url)
{
    if (!shouldAddURL(url))
        return;

    if (!image || !image->hasImage() || image->errorOccurred())
        return;

    RefPtr<SharedBuffer> data = imageLayoutObject ? image->imageForLayoutObject(imageLayoutObject)->data() : nullptr;
    if (!data)
        data = image->image()->data();

    addToResources(image, data, url);
}
