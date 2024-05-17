ShellWindowViews::~ShellWindowViews() {
  web_view_->SetWebContents(NULL);
}
