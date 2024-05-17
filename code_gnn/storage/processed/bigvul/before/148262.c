void PrintJobWorker::GetSettings(bool ask_user_for_settings,
                                 int document_page_count,
                                 bool has_selection,
                                 MarginType margin_type,
                                 bool is_scripted,
                                 bool is_modifiable) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  DCHECK_EQ(page_number_, PageNumber::npos());

  printing_context_->set_margin_type(margin_type);
  printing_context_->set_is_modifiable(is_modifiable);

  if (ask_user_for_settings) {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::BindOnce(&HoldRefCallback, make_scoped_refptr(owner_),
                       base::Bind(&PrintJobWorker::GetSettingsWithUI,
                                  base::Unretained(this), document_page_count,
                                  has_selection, is_scripted)));
  } else {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::BindOnce(&HoldRefCallback, make_scoped_refptr(owner_),
                       base::Bind(&PrintJobWorker::UseDefaultSettings,
                                  base::Unretained(this))));
  }
}
