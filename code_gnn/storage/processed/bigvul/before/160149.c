void InFlightBackendIO::OnOperationComplete(BackgroundIO* operation,
                                            bool cancel) {
  BackendIO* op = static_cast<BackendIO*>(operation);
  op->OnDone(cancel);

  if (!op->callback().is_null() && (!cancel || op->IsEntryOperation()))
    op->callback().Run(op->result());
}
