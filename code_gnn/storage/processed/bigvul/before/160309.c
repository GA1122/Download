void ImageLoader::ImageNotifyFinished(ImageResourceContent* resource) {
  RESOURCE_LOADING_DVLOG(1)
      << "ImageLoader::imageNotifyFinished " << this
      << "; has pending load event=" << pending_load_event_.IsActive();

  DCHECK(failed_load_url_.IsEmpty());
  DCHECK_EQ(resource, image_content_.Get());

  if (loading_image_document_)
    CHECK(image_complete_);
  else
    CHECK(!image_complete_);

  image_complete_ = true;
  delay_until_image_notify_finished_ = nullptr;

  if (image_content_)
    image_content_->UpdateImageAnimationPolicy();

  UpdateLayoutObject();

  if (image_content_ && image_content_->HasImage()) {
    Image& image = *image_content_->GetImage();
    if (IsHTMLImageElement(element_)) {
      Image::RecordCheckerableImageUMA(image, Image::ImageType::kImg);
    } else if (IsSVGImageElement(element_)) {
      Image::RecordCheckerableImageUMA(image, Image::ImageType::kSvg);
    }

    if (image.IsSVGImage()) {
      SVGImage& svg_image = ToSVGImage(image);
      svg_image.CheckLoaded();
      svg_image.UpdateUseCounters(GetElement()->GetDocument());
    }
  }

  DispatchDecodeRequestsIfComplete();

  if (loading_image_document_) {
    CHECK(!pending_load_event_.IsActive());
    return;
  }

  if (resource->ErrorOccurred()) {
    pending_load_event_.Cancel();

    Optional<ResourceError> error = resource->GetResourceError();
    if (error && error->IsAccessCheck())
      CrossSiteOrCSPViolationOccurred(AtomicString(error->FailingURL()));

    if (!suppress_error_events_)
      DispatchErrorEvent();
    return;
  }

  CHECK(!pending_load_event_.IsActive());
  pending_load_event_ = PostCancellableTask(
      *GetElement()->GetDocument().GetTaskRunner(TaskType::kDOMManipulation),
      FROM_HERE,
      WTF::Bind(&ImageLoader::DispatchPendingLoadEvent, WrapPersistent(this),
                WTF::Passed(IncrementLoadEventDelayCount::Create(
                    GetElement()->GetDocument()))));
}
