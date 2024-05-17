void ImageInputType::valueAttributeChanged()
{
    if (m_useFallbackContent)
        return;
    BaseButtonInputType::valueAttributeChanged();
}
