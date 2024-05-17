bool RenderBlock::updateImageLoadingPriorities()
{
    Vector<ImageResource*> images;
    appendImagesFromStyle(images, *style());

    if (images.isEmpty())
        return false;

    LayoutRect viewBounds = viewRect();
    LayoutRect objectBounds = absoluteContentBox();
    bool isVisible;
    if (!objectBounds.isEmpty())
        isVisible =  viewBounds.intersects(objectBounds);
    else
        isVisible = viewBounds.contains(objectBounds);

    ResourceLoadPriorityOptimizer::VisibilityStatus status = isVisible ?
        ResourceLoadPriorityOptimizer::Visible : ResourceLoadPriorityOptimizer::NotVisible;

    LayoutRect screenArea;
    if (!objectBounds.isEmpty()) {
        screenArea = viewBounds;
        screenArea.intersect(objectBounds);
    }

    for (Vector<ImageResource*>::iterator it = images.begin(), end = images.end(); it != end; ++it)
        ResourceLoadPriorityOptimizer::resourceLoadPriorityOptimizer()->notifyImageResourceVisibility(*it, status, screenArea);

    return true;
}
