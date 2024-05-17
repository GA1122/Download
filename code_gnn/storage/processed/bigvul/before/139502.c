static bool ExecuteScrollToBeginningOfDocument(LocalFrame& frame,
                                               Event*,
                                               EditorCommandSource,
                                               const String&) {
  return frame.GetEventHandler().BubblingScroll(kScrollBlockDirectionBackward,
                                                kScrollByDocument);
}
