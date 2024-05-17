static ResourceRequest::TargetType requestTargetType(const ResourceFetcher* fetcher, const ResourceRequest& request, Resource::Type type)
{
    switch (type) {
    case Resource::MainResource:
        if (fetcher->frame()->tree().parent())
            return ResourceRequest::TargetIsSubframe;
        return ResourceRequest::TargetIsMainFrame;
    case Resource::XSLStyleSheet:
        ASSERT(RuntimeEnabledFeatures::xsltEnabled());
    case Resource::CSSStyleSheet:
        return ResourceRequest::TargetIsStyleSheet;
    case Resource::Script:
        return ResourceRequest::TargetIsScript;
    case Resource::Font:
        return ResourceRequest::TargetIsFont;
    case Resource::Image:
        return ResourceRequest::TargetIsImage;
    case Resource::Raw:
    case Resource::ImportResource:
        return ResourceRequest::TargetIsSubresource;
    case Resource::LinkPrefetch:
        return ResourceRequest::TargetIsPrefetch;
    case Resource::LinkSubresource:
        return ResourceRequest::TargetIsSubresource;
    case Resource::TextTrack:
        return ResourceRequest::TargetIsTextTrack;
    case Resource::SVGDocument:
        return ResourceRequest::TargetIsImage;
    case Resource::Media:
        return ResourceRequest::TargetIsMedia;
    }
    ASSERT_NOT_REACHED();
    return ResourceRequest::TargetIsSubresource;
}
