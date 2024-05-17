void HTMLMediaElement::rejectScheduledPlayPromises() {
  DCHECK(m_playPromiseErrorCode == AbortError ||
         m_playPromiseErrorCode == NotSupportedError);
  if (m_playPromiseErrorCode == AbortError)
    rejectPlayPromisesInternal(
        AbortError, "The play() request was interrupted by a call to pause().");
  else
    rejectPlayPromisesInternal(
        NotSupportedError,
        "Failed to load because no supported source was found.");
}
