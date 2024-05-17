WebstoreStandaloneInstaller::CreateApproval() const {
  std::unique_ptr<WebstoreInstaller::Approval> approval(
      WebstoreInstaller::Approval::CreateWithNoInstallPrompt(
          profile_, id_,
          std::unique_ptr<base::DictionaryValue>(manifest_.get()->DeepCopy()),
          true));
  approval->skip_post_install_ui = !ShouldShowPostInstallUI();
  approval->use_app_installed_bubble = ShouldShowAppInstalledBubble();
  approval->installing_icon = gfx::ImageSkia::CreateFrom1xBitmap(icon_);
  return approval;
}
