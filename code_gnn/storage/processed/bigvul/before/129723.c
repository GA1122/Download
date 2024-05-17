static void populateResourceTiming(ResourceTimingInfo* info, Resource* resource, bool clearLoadTimings)
{
    info->setInitialRequest(resource->resourceRequest());
    info->setFinalResponse(resource->response());
    if (clearLoadTimings) {
        info->clearLoadTimings();
        info->setLoadFinishTime(info->initialTime());
    } else {
        info->setLoadFinishTime(resource->loadFinishTime());
    }
}
