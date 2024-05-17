PrintJobWorker::PrintJobWorker(int render_process_id,
                               int render_frame_id,
                               PrintJobWorkerOwner* owner)
    : owner_(owner), thread_("Printing_Worker"), weak_factory_(this) {
  DCHECK(owner_->RunsTasksInCurrentSequence());

  printing_context_delegate_ = base::MakeUnique<PrintingContextDelegate>(
      render_process_id, render_frame_id);
  printing_context_ = PrintingContext::Create(printing_context_delegate_.get());
}
