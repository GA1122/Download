void WebContentsImpl::OnEndColorChooser(RenderFrameHostImpl* source,
                                        int color_chooser_id) {
  if (color_chooser_info_ &&
      color_chooser_info_->Matches(source, color_chooser_id))
    color_chooser_info_->chooser->End();
}
