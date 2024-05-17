static bool ExecuteScrollLineUp(LocalFrame& frame,
                                Event*,
                                EditorCommandSource,
                                const String&) {
  return frame.GetEventHandler().BubblingScroll(kScrollUpIgnoringWritingMode,
                                                kScrollByLine);
}
