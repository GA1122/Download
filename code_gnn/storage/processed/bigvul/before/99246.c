void RunnableMethodTraits<printing::PrintJobWorker>::ReleaseCallee(
    printing::PrintJobWorker* obj) {
  DCHECK_EQ(owner_, obj->owner_);
  owner_ = NULL;
}
