void OnBeginFrameFinished(
    int command_id,
    const HeadlessDevToolsManagerDelegate::CommandCallback& callback,
    ImageEncoding encoding,
    int quality,
    bool has_damage,
    bool main_frame_content_updated,
    std::unique_ptr<SkBitmap> bitmap) {
  auto result = std::make_unique<base::DictionaryValue>();
  result->SetBoolean("hasDamage", has_damage);
  result->SetBoolean("mainFrameContentUpdated", main_frame_content_updated);

  if (bitmap && !bitmap->drawsNothing()) {
    result->SetString("screenshotData",
                      EncodeBitmap(*bitmap, encoding, quality));
  }

  callback.Run(CreateSuccessResponse(command_id, std::move(result)));
}
