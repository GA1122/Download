void PrintPreviewHandler::HandleSignin(const ListValue*  ) {
  gfx::NativeWindow modal_parent =
      platform_util::GetTopLevel(preview_web_contents()->GetNativeView());
  print_dialog_cloud::CreateCloudPrintSigninDialog(
      preview_web_contents()->GetBrowserContext(),
      modal_parent,
      base::Bind(&PrintPreviewHandler::OnSigninComplete, AsWeakPtr()));
}
