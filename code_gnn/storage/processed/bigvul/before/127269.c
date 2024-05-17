void PageSerializer::addToResources(Resource* resource, PassRefPtr<SharedBuffer> data, const KURL& url)
{
    if (!data) {
        LOG_ERROR("No data for resource %s", url.string().utf8().data());
        return;
    }

    String mimeType = resource->response().mimeType();
    m_resources->append(SerializedResource(url, mimeType, data));
    m_resourceURLs.add(url);
}
