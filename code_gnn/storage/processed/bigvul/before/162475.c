void ImageResource::NotifyFinished() {
  if (is_scheduling_reload_ || ShouldReloadBrokenPlaceholder())
    return;

  Resource::NotifyFinished();
}
