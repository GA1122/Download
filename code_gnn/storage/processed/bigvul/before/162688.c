void BaseRenderingContext2D::DrawImageInternal(PaintCanvas* c,
                                               CanvasImageSource* image_source,
                                               Image* image,
                                               const FloatRect& src_rect,
                                               const FloatRect& dst_rect,
                                               const PaintFlags* flags) {
  int initial_save_count = c->getSaveCount();
  PaintFlags image_flags = *flags;

  if (flags->getImageFilter()) {
    SkMatrix ctm = c->getTotalMatrix();
    SkMatrix inv_ctm;
    if (!ctm.invert(&inv_ctm)) {
      return;
    }
    c->save();
    c->concat(inv_ctm);
    SkRect bounds = dst_rect;
    ctm.mapRect(&bounds);
    PaintFlags layer_flags;
    layer_flags.setBlendMode(flags->getBlendMode());
    layer_flags.setImageFilter(flags->getImageFilter());

    c->saveLayer(&bounds, &layer_flags);
    c->concat(ctm);
    image_flags.setBlendMode(SkBlendMode::kSrcOver);
    image_flags.setImageFilter(nullptr);
  }

  if (!imageSmoothingEnabled() &&
      IsDrawScalingDown(src_rect, dst_rect,
                        GetState().Transform().XScaleSquared(),
                        GetState().Transform().YScaleSquared()))
    image_flags.setFilterQuality(kLow_SkFilterQuality);

  if (!image_source->IsVideoElement()) {
    image_flags.setAntiAlias(ShouldDrawImageAntialiased(dst_rect));
    image->Draw(c, image_flags, dst_rect, src_rect,
                kDoNotRespectImageOrientation,
                Image::kDoNotClampImageToSourceRect, Image::kSyncDecode);
  } else {
    c->save();
    c->clipRect(dst_rect);
    c->translate(dst_rect.X(), dst_rect.Y());
    c->scale(dst_rect.Width() / src_rect.Width(),
             dst_rect.Height() / src_rect.Height());
    c->translate(-src_rect.X(), -src_rect.Y());
    HTMLVideoElement* video = static_cast<HTMLVideoElement*>(image_source);
    video->PaintCurrentFrame(
        c,
        IntRect(IntPoint(), IntSize(video->videoWidth(), video->videoHeight())),
        &image_flags);
  }

  c->restoreToCount(initial_save_count);
}
