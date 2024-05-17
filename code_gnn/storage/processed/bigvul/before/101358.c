void SessionModelAssociator::UpdateSyncModelDataFromClient() {
  DCHECK(CalledOnValidThread());

  ReassociateWindows(true);
}
