bool ShouldDisableDeferral(CanvasImageSource* image_source,
                           DisableDeferralReason* reason) {
  DCHECK(reason);
  DCHECK_EQ(*reason, kDisableDeferralReasonUnknown);

  if (image_source->IsVideoElement()) {
    *reason = kDisableDeferralReasonDrawImageOfVideo;
    return true;
  }
  if (image_source->IsCanvasElement()) {
    HTMLCanvasElement* canvas = static_cast<HTMLCanvasElement*>(image_source);
    if (canvas->IsAnimated2d()) {
      *reason = kDisableDeferralReasonDrawImageOfAnimated2dCanvas;
      return true;
    }
  }
   return false;
 }
