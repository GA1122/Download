void EncodeAndReturnImage(
    ArcVoiceInteractionFrameworkService::CaptureFullscreenCallback callback,
    std::unique_ptr<ui::LayerTreeOwner> old_layer_owner,
    gfx::Image image) {
  old_layer_owner.reset();
  base::PostTaskWithTraitsAndReplyWithResult(
      FROM_HERE,
      base::TaskTraits{base::MayBlock(), base::TaskPriority::USER_BLOCKING},
      base::BindOnce(
          [](SkBitmap image) -> std::vector<uint8_t> {
            std::vector<uint8_t> res;
            gfx::JPEGCodec::Encode(image, 100, &res);
            return res;
          },
          image.AsBitmap()),
      std::move(callback));
}
