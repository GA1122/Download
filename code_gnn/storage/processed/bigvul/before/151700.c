void Browser::MoveValidationMessage(content::WebContents* web_contents,
                                    const gfx::Rect& anchor_in_root_view) {
  if (!validation_message_bubble_)
    return;
  RenderWidgetHostView* rwhv = web_contents->GetRenderWidgetHostView();
  if (rwhv) {
    validation_message_bubble_->SetPositionRelativeToAnchor(
        rwhv->GetRenderWidgetHost(), anchor_in_root_view);
  }
}
