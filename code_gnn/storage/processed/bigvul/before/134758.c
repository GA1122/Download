void GuestViewBase::UpdatePreferredSize(
    content::WebContents* target_web_contents,
    const gfx::Size& pref_size) {
  DCHECK_EQ(web_contents(), target_web_contents);
  if (IsPreferredSizeModeEnabled()) {
    OnPreferredSizeChanged(pref_size);
  }
}
