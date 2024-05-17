bool RenderSVGImage::updateImageViewport()
{
    SVGImageElement* image = toSVGImageElement(element());
    FloatRect oldBoundaries = m_objectBoundingBox;
    bool updatedViewport = false;

    SVGLengthContext lengthContext(image);
    m_objectBoundingBox = FloatRect(image->xCurrentValue().value(lengthContext), image->yCurrentValue().value(lengthContext), image->widthCurrentValue().value(lengthContext), image->heightCurrentValue().value(lengthContext));

    if (image->preserveAspectRatioCurrentValue().align() == SVGPreserveAspectRatio::SVG_PRESERVEASPECTRATIO_NONE) {
        if (ImageResource* cachedImage = m_imageResource->cachedImage()) {
            LayoutSize intrinsicSize = cachedImage->imageSizeForRenderer(0, style()->effectiveZoom());
            if (intrinsicSize != m_imageResource->imageSize(style()->effectiveZoom())) {
                m_imageResource->setContainerSizeForRenderer(roundedIntSize(intrinsicSize));
                updatedViewport = true;
            }
        }
    }

    if (oldBoundaries != m_objectBoundingBox) {
        if (!updatedViewport)
            m_imageResource->setContainerSizeForRenderer(enclosingIntRect(m_objectBoundingBox).size());
        updatedViewport = true;
        m_needsBoundariesUpdate = true;
    }

    return updatedViewport;
}
