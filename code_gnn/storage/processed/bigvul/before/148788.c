void InterstitialPageImpl::Hide() {
  if (!render_view_host_)
    return;

  Disable();

  RenderWidgetHostView* old_view =
      controller_->delegate()->GetRenderViewHost()->GetWidget()->GetView();
  if (controller_->delegate()->GetInterstitialPage() == this &&
      old_view &&
      !old_view->IsShowing() &&
      !controller_->delegate()->IsHidden()) {
    old_view->Show();
  }

  if (render_view_host_->GetWidget()->GetView() &&
      render_view_host_->GetWidget()->GetView()->HasFocus() &&
      controller_->delegate()->GetRenderViewHost()->GetWidget()->GetView()) {
    controller_->delegate()
        ->GetRenderViewHost()
        ->GetWidget()
        ->GetView()
        ->Focus();
  }

  base::ThreadTaskRunnerHandle::Get()->PostNonNestableTask(
      FROM_HERE, base::Bind(&InterstitialPageImpl::Shutdown,
                            weak_ptr_factory_.GetWeakPtr()));
  render_view_host_ = NULL;
  frame_tree_->root()->ResetForNewProcess();
  controller_->delegate()->DetachInterstitialPage();
  NavigationEntry* entry = controller_->GetVisibleEntry();
  if (entry && !new_navigation_ && should_revert_web_contents_title_)
    web_contents_->UpdateTitleForEntry(entry, original_web_contents_title_);

  static_cast<WebContentsImpl*>(web_contents_)->DidChangeVisibleSecurityState();

  InterstitialPageMap::iterator iter =
      g_web_contents_to_interstitial_page->find(web_contents_);
  DCHECK(iter != g_web_contents_to_interstitial_page->end());
  if (iter != g_web_contents_to_interstitial_page->end())
    g_web_contents_to_interstitial_page->erase(iter);

  web_contents_ = NULL;
}
