void ResourceFetcher::storeResourceTimingInitiatorInformation(Resource* resource)
{
    if (resource->options().requestInitiatorContext != DocumentContext)
        return;

    RefPtr<ResourceTimingInfo> info = ResourceTimingInfo::create(resource->options().initiatorInfo.name, monotonicallyIncreasingTime());

    if (resource->isCacheValidator()) {
        const AtomicString& timingAllowOrigin = resource->resourceToRevalidate()->response().httpHeaderField("Timing-Allow-Origin");
        if (!timingAllowOrigin.isEmpty())
            info->setOriginalTimingAllowOrigin(timingAllowOrigin);
    }

    if (resource->type() == Resource::MainResource) {
        if (frame()->deprecatedLocalOwner() && !frame()->deprecatedLocalOwner()->loadedNonEmptyDocument()) {
            info->setInitiatorType(frame()->deprecatedLocalOwner()->localName());
            m_resourceTimingInfoMap.add(resource, info);
            frame()->deprecatedLocalOwner()->didLoadNonEmptyDocument();
        }
    } else {
        m_resourceTimingInfoMap.add(resource, info);
    }
}
