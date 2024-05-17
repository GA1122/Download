ExternalProtocolDialog::ExternalProtocolDialog(
    std::unique_ptr<const ProtocolDialogDelegate> delegate,
    int render_process_host_id,
    int routing_id)
    : delegate_(std::move(delegate)),
      render_process_host_id_(render_process_host_id),
      routing_id_(routing_id),
      creation_time_(base::TimeTicks::Now()) {
  ChromeLayoutProvider* provider = ChromeLayoutProvider::Get();
  set_margins(
      provider->GetDialogInsetsForContentType(views::TEXT, views::TEXT));

  SetLayoutManager(std::make_unique<views::FillLayout>());

  DCHECK(delegate_->GetMessageText().empty());
  remember_decision_checkbox_ =
      new views::Checkbox(delegate_->GetCheckboxText());
  AddChildView(remember_decision_checkbox_);

  WebContents* web_contents = tab_util::GetWebContentsByID(
      render_process_host_id_, routing_id_);
  if (web_contents)
    constrained_window::ShowWebModalDialogViews(this, web_contents);
  chrome::RecordDialogCreation(chrome::DialogIdentifier::EXTERNAL_PROTOCOL);
}
