void TypingCommand::deleteKeyPressed(TextGranularity granularity,
                                     bool killRing,
                                     EditingState* editingState) {
  LocalFrame* frame = document().frame();
  if (!frame)
    return;

  frame->spellChecker().updateMarkersForWordsAffectedByEditing(false);

  VisibleSelection selectionToDelete;
  VisibleSelection selectionAfterUndo;

  switch (endingSelection().getSelectionType()) {
    case RangeSelection:
      selectionToDelete = endingSelection();
      selectionAfterUndo = selectionToDelete;
      break;
    case CaretSelection: {
      bool breakOutResult =
          breakOutOfEmptyMailBlockquotedParagraph(editingState);
      if (editingState->isAborted())
        return;
      if (breakOutResult)
        typingAddedToOpenCommand(DeleteKey);

      m_smartDelete = false;
      document().updateStyleAndLayoutIgnorePendingStylesheets();

      SelectionModifier selectionModifier(*frame, endingSelection());
      selectionModifier.modify(FrameSelection::AlterationExtend,
                               DirectionBackward, granularity);
      if (killRing && selectionModifier.selection().isCaret() &&
          granularity != CharacterGranularity)
        selectionModifier.modify(FrameSelection::AlterationExtend,
                                 DirectionBackward, CharacterGranularity);

      VisiblePosition visibleStart(endingSelection().visibleStart());
      if (previousPositionOf(visibleStart, CannotCrossEditingBoundary)
              .isNull()) {
        bool breakOutOfEmptyListItemResult =
            breakOutOfEmptyListItem(editingState);
        if (editingState->isAborted())
          return;
        if (breakOutOfEmptyListItemResult) {
          typingAddedToOpenCommand(DeleteKey);
          return;
        }
        if (nextPositionOf(visibleStart, CannotCrossEditingBoundary).isNull() &&
            makeEditableRootEmpty(editingState)) {
          typingAddedToOpenCommand(DeleteKey);
          return;
        }
        if (editingState->isAborted())
          return;
      }

      Node* enclosingTableCell =
          enclosingNodeOfType(visibleStart.deepEquivalent(), &isTableCell);
      if (enclosingTableCell &&
          visibleStart.deepEquivalent() ==
              VisiblePosition::firstPositionInNode(enclosingTableCell)
                  .deepEquivalent())
        return;

      if (isStartOfParagraph(visibleStart) &&
          tableElementJustBefore(
              previousPositionOf(visibleStart, CannotCrossEditingBoundary))) {
        if (tableElementJustAfter(visibleStart))
          return;
        selectionModifier.modify(FrameSelection::AlterationExtend,
                                 DirectionBackward, granularity);
      } else if (Element* table = tableElementJustBefore(visibleStart)) {
        setEndingSelection(
            SelectionInDOMTree::Builder()
                .collapse(Position::beforeNode(table))
                .extend(endingSelection().start())
                .setIsDirectional(endingSelection().isDirectional())
                .build());
        typingAddedToOpenCommand(DeleteKey);
        return;
      }

      selectionToDelete = selectionModifier.selection();

      if (granularity == CharacterGranularity &&
          selectionToDelete.end().computeContainerNode() ==
              selectionToDelete.start().computeContainerNode() &&
          selectionToDelete.end().computeOffsetInContainerNode() -
                  selectionToDelete.start().computeOffsetInContainerNode() >
              1) {
        selectionToDelete.setWithoutValidation(
            selectionToDelete.end(),
            previousPositionOf(selectionToDelete.end(),
                               PositionMoveType::BackwardDeletion));
      }

      if (!startingSelection().isRange() ||
          selectionToDelete.base() != startingSelection().start()) {
        selectionAfterUndo = selectionToDelete;
      } else {
        selectionAfterUndo.setWithoutValidation(startingSelection().end(),
                                                selectionToDelete.extent());
      }
      break;
    }
    case NoSelection:
      NOTREACHED();
      break;
  }

  DCHECK(!selectionToDelete.isNone());
  if (selectionToDelete.isNone())
    return;

  if (selectionToDelete.isCaret())
    return;

  if (killRing)
    frame->editor().addToKillRing(
        selectionToDelete.toNormalizedEphemeralRange());
  if (frame->editor().behavior().shouldUndoOfDeleteSelectText() &&
      m_openedByBackwardDelete)
    setStartingSelection(selectionAfterUndo);
  deleteSelectionIfRange(selectionToDelete, editingState, m_smartDelete);
  if (editingState->isAborted())
    return;
  setSmartDelete(false);
  typingAddedToOpenCommand(DeleteKey);
}
