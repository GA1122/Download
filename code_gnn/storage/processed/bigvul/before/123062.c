bool ShouldCoalesceMouseWheelEvents(const WebMouseWheelEvent& last_event,
                                    const WebMouseWheelEvent& new_event) {
  return last_event.modifiers == new_event.modifiers &&
         last_event.scrollByPage == new_event.scrollByPage &&
         last_event.hasPreciseScrollingDeltas
             == new_event.hasPreciseScrollingDeltas &&
         last_event.phase == new_event.phase &&
         last_event.momentumPhase == new_event.momentumPhase;
}
