bool RenderWidgetHostImpl::GetScreenColorProfile(
    std::vector<char>* color_profile) {
  DCHECK(color_profile->empty());
  if (view_)
    return view_->GetScreenColorProfile(color_profile);
  return false;
}
