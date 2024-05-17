PrintJobWorker* PrinterQuery::DetachWorker(PrintJobWorkerOwner* new_owner) {
  DCHECK(!callback_.get());
  DCHECK(worker_.get());
  if (!worker_.get())
    return NULL;
  worker_->SetNewOwner(new_owner);
  return worker_.release();
 }
