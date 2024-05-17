void HoldRefCallback(const scoped_refptr<PrintJobWorkerOwner>& owner,
                     const base::Closure& callback) {
  callback.Run();
}
