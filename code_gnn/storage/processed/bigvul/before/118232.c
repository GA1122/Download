gfx::Size AutofillDialogViews::GetMaximumSignInViewSize() const {
  web_modal::WebContentsModalDialogHost* dialog_host =
      web_modal::WebContentsModalDialogManager::FromWebContents(
          delegate_->GetWebContents())->delegate()->
              GetWebContentsModalDialogHost();

  int height = dialog_host->GetMaximumDialogSize().height();
  const int non_client_height = GetWidget()->non_client_view()->height();
  const int client_height = GetWidget()->client_view()->height();
  height -= non_client_height - client_height - 12;
  height = std::max(height, kMinimumContentsHeight);

  const int width = GetDialogClientView()->size().width() - GetInsets().width();
  return gfx::Size(width, height);
}
