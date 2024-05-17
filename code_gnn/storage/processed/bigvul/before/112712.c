void DocumentLoader::subresourceLoaderFinishedLoadingOnePart(ResourceLoader* loader)
{
    m_multipartSubresourceLoaders.add(loader);
    m_subresourceLoaders.remove(loader);
    checkLoadComplete();
    if (Frame* frame = m_frame)
        frame->loader()->checkLoadComplete();    
}
