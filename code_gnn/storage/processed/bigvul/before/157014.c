void WebMediaPlayerMS::TriggerResize() {
  if (HasVideo())
    get_client()->SizeChanged();

  delegate_->DidPlayerSizeChange(delegate_id_, NaturalSize());
}
