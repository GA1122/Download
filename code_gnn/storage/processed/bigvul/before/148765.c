void InterstitialPageImpl::DidNavigate(
    RenderViewHost* render_view_host,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params) {
  if (!enabled()) {
    DontProceed();
    return;
  }
  if (ui::PageTransitionCoreTypeIs(params.transition,
                                   ui::PAGE_TRANSITION_AUTO_SUBFRAME)) {
    return;
  }

  pause_throbber_ = true;

  if (!controller_->delegate()->IsHidden())
    render_view_host_->GetWidget()->GetView()->Show();
  controller_->delegate()->AttachInterstitialPage(this);

  RenderWidgetHostView* rwh_view =
      controller_->delegate()->GetRenderViewHost()->GetWidget()->GetView();

  if (rwh_view) {
    if (rwh_view->HasFocus())
      Focus();

    rwh_view->Hide();
  }
}
