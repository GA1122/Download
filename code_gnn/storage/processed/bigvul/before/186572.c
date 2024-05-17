  PaintImage AcceleratedStaticBitmapImage::PaintImageForCurrentFrame() {
  CheckThread();
//   DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
    if (!IsValid())
      return PaintImage();
  
    sk_sp<SkImage> image;
    if (original_skia_image_ &&
      original_skia_image_thread_id_ ==
          Platform::Current()->CurrentThread()->ThreadId()) {
//       original_skia_image_task_runner_->BelongsToCurrentThread()) {
     image = original_skia_image_;
   } else {
     CreateImageFromMailboxIfNeeded();
     image = texture_holder_->GetSkImage();
   }
 
   return CreatePaintImageBuilder()
       .set_image(image, paint_image_content_id_)
       .set_completion_state(PaintImage::CompletionState::DONE)
       .TakePaintImage();
 }