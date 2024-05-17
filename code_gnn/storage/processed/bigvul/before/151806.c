void BackgroundLoaderOffliner::TerminateLoadIfInProgress() {
  if (!pending_request_)
    return;

  Cancel(base::BindOnce(HandleLoadTerminationCancel,
                        std::move(completion_callback_)));
}
