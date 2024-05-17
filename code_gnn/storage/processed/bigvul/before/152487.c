void RecordReadyToCommitUntilCommitHistogram(base::TimeDelta delay,
                                             ui::PageTransition transition) {
  UMA_HISTOGRAM_TIMES("Navigation.Renderer.ReadyToCommitUntilCommit", delay);
  if (transition & ui::PAGE_TRANSITION_FORWARD_BACK) {
    UMA_HISTOGRAM_TIMES(
        "Navigation.Renderer.ReadyToCommitUntilCommit.BackForward", delay);
  } else if (ui::PageTransitionCoreTypeIs(transition,
                                          ui::PAGE_TRANSITION_RELOAD)) {
    UMA_HISTOGRAM_TIMES("Navigation.Renderer.ReadyToCommitUntilCommit.Reload",
                        delay);
  } else if (ui::PageTransitionIsNewNavigation(transition)) {
    UMA_HISTOGRAM_TIMES(
        "Navigation.Renderer.ReadyToCommitUntilCommit.NewNavigation", delay);
  } else {
    NOTREACHED() << "Invalid page transition: " << transition;
  }
}
