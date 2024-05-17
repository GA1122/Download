void HTMLMediaElement::RejectScheduledPlayPromises() {
  DCHECK(play_promise_error_code_ == kAbortError ||
         play_promise_error_code_ == kNotSupportedError);
  if (play_promise_error_code_ == kAbortError) {
    RecordPlayPromiseRejected(PlayPromiseRejectReason::kInterruptedByPause);
    RejectPlayPromisesInternal(kAbortError,
                               "The play() request was interrupted by a call "
                               "to pause(). https://goo.gl/LdLk22");
  } else {
    RecordPlayPromiseRejected(PlayPromiseRejectReason::kNoSupportedSources);
    RejectPlayPromisesInternal(
        kNotSupportedError,
        "Failed to load because no supported source was found.");
  }
}
