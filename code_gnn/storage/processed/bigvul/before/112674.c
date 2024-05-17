PassRefPtr<ArchiveResource> DocumentLoader::mainResource() const
{
    const ResourceResponse& r = response();
    
    RefPtr<ResourceBuffer> mainResourceBuffer = mainResourceData();
    RefPtr<SharedBuffer> data = mainResourceBuffer ? mainResourceBuffer->sharedBuffer() : 0;
    if (!data)
        data = SharedBuffer::create();
        
    return ArchiveResource::create(data, r.url(), r.mimeType(), r.textEncodingName(), frame()->tree()->uniqueName());
}
