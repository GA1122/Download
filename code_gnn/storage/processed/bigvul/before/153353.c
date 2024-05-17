bool Tab::ShouldRenderAsNormalTab() const {
  return !data().pinned || (width() >= (TabStyle::GetPinnedWidth() +
                                        kPinnedTabExtraWidthToRenderAsNormal));
}
