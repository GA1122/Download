void ImageBitmapFactories::ImageBitmapLoader::ScheduleAsyncImageBitmapDecoding(
    DOMArrayBuffer* array_buffer) {
  scoped_refptr<base::SingleThreadTaskRunner> task_runner =
      Thread::Current()->GetTaskRunner();
  background_scheduler::PostOnBackgroundThread(
      FROM_HERE,
      CrossThreadBind(
          &ImageBitmapFactories::ImageBitmapLoader::DecodeImageOnDecoderThread,
          WrapCrossThreadPersistent(this), std::move(task_runner),
          WrapCrossThreadPersistent(array_buffer), options_->premultiplyAlpha(),
          options_->colorSpaceConversion()));
}
