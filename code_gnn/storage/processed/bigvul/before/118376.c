void CardUnmaskPromptViews::Show() {
  constrained_window::ShowWebModalDialogViews(this,
                                              controller_->GetWebContents());
}
