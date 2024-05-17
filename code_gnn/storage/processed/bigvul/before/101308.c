SessionChangeProcessor::SessionChangeProcessor(
    UnrecoverableErrorHandler* error_handler,
    SessionModelAssociator* session_model_associator,
    bool setup_for_test)
    : ChangeProcessor(error_handler),
      session_model_associator_(session_model_associator),
      profile_(NULL),
      setup_for_test_(setup_for_test) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(error_handler);
  DCHECK(session_model_associator_);
}
