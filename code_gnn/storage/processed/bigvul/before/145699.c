void ImageInputType::startResourceLoading()
{
    BaseButtonInputType::startResourceLoading();

    HTMLImageLoader& imageLoader = element().ensureImageLoader();
    imageLoader.updateFromElement();

    LayoutObject* layoutObject = element().layoutObject();
    if (!layoutObject || !layoutObject->isLayoutImage())
        return;

    LayoutImageResource* imageResource = toLayoutImage(layoutObject)->imageResource();
    imageResource->setImageResource(imageLoader.image());
}
