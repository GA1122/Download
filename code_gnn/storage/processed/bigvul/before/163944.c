void PaymentHandlerWebFlowViewController::FillContentView(
    views::View* content_view) {
  content_view->SetLayoutManager(std::make_unique<views::FillLayout>());
  std::unique_ptr<views::WebView> web_view =
      std::make_unique<views::WebView>(profile_);
  Observe(web_view->GetWebContents());
  web_contents()->SetDelegate(this);
  web_view->LoadInitialURL(target_);

  dialog_manager_delegate_.SetWebContents(web_contents());
  web_modal::WebContentsModalDialogManager::CreateForWebContents(
      web_contents());
  web_modal::WebContentsModalDialogManager::FromWebContents(web_contents())
      ->SetDelegate(&dialog_manager_delegate_);

  web_view->SetPreferredSize(gfx::Size(0, kDialogHeight - 75));
  content_view->AddChildView(web_view.release());
}
