bool ImageTokenizer::writeRawData(const char* data, int len)
{
    if (!m_imageElement)
        createDocumentStructure();

    CachedImage* cachedImage = m_imageElement->cachedImage();
    cachedImage->data(cachedImage->bufferData(data, len, 0), false);

    return false;
}
