unsigned ImageInputType::width() const
{
    RefPtrWillBeRawPtr<HTMLInputElement> element(this->element());

    if (!element->layoutObject()) {
        unsigned width;
        if (parseHTMLNonNegativeInteger(element->fastGetAttribute(widthAttr), width))
            return width;

        HTMLImageLoader* imageLoader = element->imageLoader();
        if (imageLoader && imageLoader->image())
            return imageLoader->image()->imageSize(LayoutObject::shouldRespectImageOrientation(nullptr), 1).width();
    }

    element->document().updateLayout();

    LayoutBox* box = element->layoutBox();
    return box ? adjustForAbsoluteZoom(box->contentWidth(), box) : 0;
}
