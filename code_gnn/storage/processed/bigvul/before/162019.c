void BrowserChildProcessHostImpl::HistogramBadMessageTerminated(
    ProcessType process_type) {
  UMA_HISTOGRAM_ENUMERATION("ChildProcess.BadMessgeTerminated", process_type,
                            PROCESS_TYPE_MAX);
}
