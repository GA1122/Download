void PrintPreviewHandler::HandleManageCloudPrint(const ListValue*  ) {
  ++manage_cloud_printers_dialog_request_count_;
  Profile* profile = Profile::FromBrowserContext(
      preview_web_contents()->GetBrowserContext());
  preview_web_contents()->OpenURL(
      OpenURLParams(
          CloudPrintURL(profile).GetCloudPrintServiceManageURL(),
          Referrer(),
          NEW_FOREGROUND_TAB,
          content::PAGE_TRANSITION_LINK,
          false));
}
