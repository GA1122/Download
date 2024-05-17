void DocumentLoader::setDefersLoading(bool defers)
{
    if (mainResourceLoader() && mainResourceLoader()->documentLoader() == this)
        mainResourceLoader()->setDefersLoading(defers);

    setAllDefersLoading(m_subresourceLoaders, defers);
    setAllDefersLoading(m_plugInStreamLoaders, defers);
    if (!defers)
        deliverSubstituteResourcesAfterDelay();
}
