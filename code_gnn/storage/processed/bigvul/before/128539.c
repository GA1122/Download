pp::ImageData Instance::CreateResourceImage(PP_ResourceImage image_id) {
  pp::ImageData resource_data;
  if (hidpi_enabled_) {
    resource_data =
        pp::PDF::GetResourceImageForScale(this, image_id, device_scale_);
  }

  return resource_data.data() ? resource_data
                              : pp::PDF::GetResourceImage(this, image_id);
}
