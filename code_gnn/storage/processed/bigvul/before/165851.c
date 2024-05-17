void ImageBitmapFactories::ImageBitmapLoader::DecodeImageOnDecoderThread(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner,
    DOMArrayBuffer* array_buffer,
    const String& premultiply_alpha_option,
    const String& color_space_conversion_option) {
  DCHECK(!IsMainThread());

  ImageDecoder::AlphaOption alpha_op = ImageDecoder::kAlphaPremultiplied;
  if (premultiply_alpha_option == "none")
    alpha_op = ImageDecoder::kAlphaNotPremultiplied;
  bool ignore_color_space = false;
  if (color_space_conversion_option == "none")
    ignore_color_space = true;
  const bool data_complete = true;
  std::unique_ptr<ImageDecoder> decoder(ImageDecoder::Create(
      SegmentReader::CreateFromSkData(SkData::MakeWithoutCopy(
          array_buffer->Data(), array_buffer->ByteLength())),
      data_complete, alpha_op, ImageDecoder::kDefaultBitDepth,
      ignore_color_space ? ColorBehavior::Ignore() : ColorBehavior::Tag()));
  sk_sp<SkImage> frame;
  if (decoder) {
    frame = ImageBitmap::GetSkImageFromDecoder(std::move(decoder));
  }
  PostCrossThreadTask(
      *task_runner, FROM_HERE,
      CrossThreadBind(&ImageBitmapFactories::ImageBitmapLoader::
                          ResolvePromiseOnOriginalThread,
                      WrapCrossThreadPersistent(this), std::move(frame)));
}
