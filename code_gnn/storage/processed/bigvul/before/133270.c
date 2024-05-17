RenderSVGImage::RenderSVGImage(SVGImageElement* impl)
    : RenderSVGModelObject(impl)
    , m_needsBoundariesUpdate(true)
    , m_needsTransformUpdate(true)
    , m_imageResource(RenderImageResource::create())
{
    m_imageResource->initialize(this);
}
