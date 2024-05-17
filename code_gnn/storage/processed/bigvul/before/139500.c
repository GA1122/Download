static bool ExecuteScrollPageBackward(LocalFrame& frame,
                                      Event*,
                                      EditorCommandSource,
                                      const String&) {
  return frame.GetEventHandler().BubblingScroll(kScrollBlockDirectionBackward,
                                                kScrollByPage);
}
