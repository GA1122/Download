void PostOnOwnerThread(const scoped_refptr<PrintJobWorkerOwner>& owner,
                       const PrintingContext::PrintSettingsCallback& callback,
                       PrintingContext::Result result) {
  owner->PostTask(FROM_HERE, base::Bind(&HoldRefCallback, owner,
                                        base::Bind(callback, result)));
}
