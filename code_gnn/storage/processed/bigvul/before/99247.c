void RunnableMethodTraits<printing::PrintJobWorker>::RetainCallee(
    printing::PrintJobWorker* obj) {
  DCHECK(!owner_.get());
  owner_ = obj->owner_;
}
