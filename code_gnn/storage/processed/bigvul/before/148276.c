void PrintJobWorker::SetSettings(
    std::unique_ptr<base::DictionaryValue> new_settings) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());

  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::BindOnce(
          &HoldRefCallback, make_scoped_refptr(owner_),
          base::Bind(&PrintJobWorker::UpdatePrintSettings,
                     base::Unretained(this), base::Passed(&new_settings))));
}
