 void HandleLoadTerminationCancel(
     Offliner::CompletionCallback completion_callback,
     const SavePageRequest& canceled_request) {
  std::move(completion_callback)
      .Run(canceled_request, Offliner::RequestStatus::FOREGROUND_CANCELED);
}
