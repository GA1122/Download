void ImageLoader::UpdateFromElement(UpdateFromElementBehavior update_behavior,
                                    ReferrerPolicy referrer_policy) {
  AtomicString image_source_url = element_->ImageSourceURL();
  suppress_error_events_ = (update_behavior == kUpdateSizeChanged);

  if (update_behavior == kUpdateIgnorePreviousError)
    ClearFailedLoadURL();

  if (!failed_load_url_.IsEmpty() && image_source_url == failed_load_url_)
    return;

  if (loading_image_document_ && update_behavior == kUpdateForcedReload) {
    loading_image_document_ = false;
    image_resource_for_image_document_ = nullptr;
    ClearImage();
  }

  if (loading_image_document_) {
    ResourceRequest request(ImageSourceToKURL(element_->ImageSourceURL()));
    request.SetFetchCredentialsMode(
        network::mojom::FetchCredentialsMode::kOmit);
    ImageResource* image_resource = ImageResource::Create(request);
    image_resource->SetStatus(ResourceStatus::kPending);
    image_resource->NotifyStartLoad();
    SetImageForImageDocument(image_resource);
    return;
  }

  if (pending_task_) {
    pending_task_->ClearLoader();
    pending_task_.reset();
    delay_until_do_update_from_element_ = nullptr;
  }

  KURL url = ImageSourceToKURL(image_source_url);
  if (ShouldLoadImmediately(url)) {
    DoUpdateFromElement(kDoNotBypassMainWorldCSP, update_behavior, url,
                        referrer_policy, UpdateType::kSync);
    return;
  }
  if (image_source_url.IsEmpty()) {
    ImageResourceContent* image = image_content_.Get();
    if (image) {
      image->RemoveObserver(this);
    }
    image_content_ = nullptr;
    image_resource_for_image_document_ = nullptr;
    delay_until_image_notify_finished_ = nullptr;
  }

  Document& document = element_->GetDocument();
  if (document.IsActive())
    EnqueueImageLoadingMicroTask(update_behavior, referrer_policy);
}
