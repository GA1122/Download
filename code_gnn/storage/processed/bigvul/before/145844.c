void OnBeginFrameFinished(
    int command_id,
    const HeadlessDevToolsManagerDelegate::CommandCallback& callback,
    ImageEncoding encoding,
    int quality,
    bool has_damage,
    std::unique_ptr<SkBitmap> bitmap) {
  auto result = base::MakeUnique<base::DictionaryValue>();
  result->SetBoolean("hasDamage", has_damage);

  if (bitmap && !bitmap->drawsNothing()) {
    result->SetString("screenshotData",
                      EncodeBitmap(*bitmap, encoding, quality));
  }

  callback.Run(CreateSuccessResponse(command_id, std::move(result)));
}
