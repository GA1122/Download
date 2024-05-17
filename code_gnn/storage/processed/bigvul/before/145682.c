LayoutObject* ImageInputType::createLayoutObject(const ComputedStyle& style) const
{
    if (m_useFallbackContent)
        return new LayoutBlockFlow(&element());
    LayoutImage* image = new LayoutImage(&element());
    image->setImageResource(LayoutImageResource::create());
    return image;
}
