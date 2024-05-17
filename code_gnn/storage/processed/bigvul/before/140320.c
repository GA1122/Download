bool Editor::deleteWithDirection(DeleteDirection direction,
                                 TextGranularity granularity,
                                 bool killRing,
                                 bool isTypingAction) {
  if (!canEdit())
    return false;

  EditingState editingState;
  if (frame()
          .selection()
          .computeVisibleSelectionInDOMTreeDeprecated()
          .isRange()) {
    if (isTypingAction) {
      DCHECK(frame().document());
      TypingCommand::deleteKeyPressed(
          *frame().document(),
          canSmartCopyOrDelete() ? TypingCommand::SmartDelete : 0, granularity);
      revealSelectionAfterEditingOperation();
    } else {
      if (killRing)
        addToKillRing(selectedRange());
      deleteSelectionWithSmartDelete(
          canSmartCopyOrDelete() ? DeleteMode::Smart : DeleteMode::Simple,
          deletionInputTypeFromTextGranularity(direction, granularity));
    }
  } else {
    TypingCommand::Options options = 0;
    if (canSmartCopyOrDelete())
      options |= TypingCommand::SmartDelete;
    if (killRing)
      options |= TypingCommand::KillRing;
    switch (direction) {
      case DeleteDirection::Forward:
        DCHECK(frame().document());
        TypingCommand::forwardDeleteKeyPressed(
            *frame().document(), &editingState, options, granularity);
        if (editingState.isAborted())
          return false;
        break;
      case DeleteDirection::Backward:
        DCHECK(frame().document());
        TypingCommand::deleteKeyPressed(*frame().document(), options,
                                        granularity);
        break;
    }
    revealSelectionAfterEditingOperation();
  }

  if (killRing)
    setStartNewKillRingSequence(false);

  return true;
}
