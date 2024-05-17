ExtensionInstallDialogView::ExtensionInstallDialogView(
    Profile* profile,
    content::PageNavigator* navigator,
    ExtensionInstallPrompt::Delegate* delegate,
    scoped_refptr<ExtensionInstallPrompt::Prompt> prompt)
    : profile_(profile),
      navigator_(navigator),
      delegate_(delegate),
      prompt_(prompt),
      container_(NULL),
      scroll_view_(NULL),
      handled_result_(false) {
  InitView();
}
