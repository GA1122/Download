void BackgroundContentsService::BackgroundContentsShutdown(
    BackgroundContents* background_contents) {
  DCHECK(IsTracked(background_contents));
  string16 appid = GetParentApplicationId(background_contents);
  contents_map_.erase(appid);
}
