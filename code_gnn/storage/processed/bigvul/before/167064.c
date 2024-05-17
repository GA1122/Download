  views::DialogDelegateView* CreateAndShowPrompt(
      ExtensionInstallPromptTestHelper* helper) {
    std::unique_ptr<ExtensionInstallDialogView> dialog(
        new ExtensionInstallDialogView(profile(), web_contents(),
                                       helper->GetCallback(), CreatePrompt()));
    views::DialogDelegateView* delegate_view = dialog.get();

    views::Widget* modal_dialog = views::DialogDelegate::CreateDialogWidget(
        dialog.release(), nullptr,
        platform_util::GetViewForWindow(
            browser()->window()->GetNativeWindow()));
    modal_dialog->Show();

    return delegate_view;
  }
