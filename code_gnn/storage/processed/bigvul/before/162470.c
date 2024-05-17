ImageResource::ImageResource(const ResourceRequest& resource_request,
                             const ResourceLoaderOptions& options,
                             ImageResourceContent* content,
                             bool is_placeholder)
    : Resource(resource_request, kImage, options),
      content_(content),
      device_pixel_ratio_header_value_(1.0),
      has_device_pixel_ratio_header_value_(false),
      is_scheduling_reload_(false),
      placeholder_option_(
          is_placeholder ? PlaceholderOption::kShowAndReloadPlaceholderAlways
                         : PlaceholderOption::kDoNotReloadPlaceholder),
      flush_timer_(this, &ImageResource::FlushImageIfNeeded) {
  DCHECK(GetContent());
  RESOURCE_LOADING_DVLOG(1) << "new ImageResource(ResourceRequest) " << this;
  GetContent()->SetImageResourceInfo(new ImageResourceInfoImpl(this));
}
