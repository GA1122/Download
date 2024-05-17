bool Editor::DeleteWithDirection(DeleteDirection direction,
                                 TextGranularity granularity,
                                 bool kill_ring,
                                 bool is_typing_action) {
  if (!CanEdit())
    return false;

  EditingState editing_state;
  if (GetFrame()
          .Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .IsRange()) {
    if (is_typing_action) {
      DCHECK(GetFrame().GetDocument());
      TypingCommand::DeleteKeyPressed(
          *GetFrame().GetDocument(),
          CanSmartCopyOrDelete() ? TypingCommand::kSmartDelete : 0,
          granularity);
      RevealSelectionAfterEditingOperation();
    } else {
      if (kill_ring)
        AddToKillRing(SelectedRange());
      DeleteSelectionWithSmartDelete(
          CanSmartCopyOrDelete() ? DeleteMode::kSmart : DeleteMode::kSimple,
          DeletionInputTypeFromTextGranularity(direction, granularity));
    }
  } else {
    TypingCommand::Options options = 0;
    if (CanSmartCopyOrDelete())
      options |= TypingCommand::kSmartDelete;
    if (kill_ring)
      options |= TypingCommand::kKillRing;
    switch (direction) {
      case DeleteDirection::kForward:
        DCHECK(GetFrame().GetDocument());
        TypingCommand::ForwardDeleteKeyPressed(
            *GetFrame().GetDocument(), &editing_state, options, granularity);
        if (editing_state.IsAborted())
          return false;
        break;
      case DeleteDirection::kBackward:
        DCHECK(GetFrame().GetDocument());
        TypingCommand::DeleteKeyPressed(*GetFrame().GetDocument(), options,
                                        granularity);
        break;
    }
    RevealSelectionAfterEditingOperation();
  }

  if (kill_ring)
    SetStartNewKillRingSequence(false);

  return true;
}
