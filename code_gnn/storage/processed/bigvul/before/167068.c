bool ScrollbarTest::IsScrollbarVisible(
    std::unique_ptr<ExtensionInstallPrompt::Prompt> prompt) {
  ExtensionInstallDialogView* dialog = new ExtensionInstallDialogView(
      profile(), web_contents(), ExtensionInstallPrompt::DoneCallback(),
      std::move(prompt));

  views::Widget* modal = constrained_window::CreateBrowserModalDialogViews(
      dialog, web_contents()->GetTopLevelNativeWindow());
  modal->Show();
  content::RunAllBlockingPoolTasksUntilIdle();

  return dialog->scroll_view()->vertical_scroll_bar()->visible();
}
