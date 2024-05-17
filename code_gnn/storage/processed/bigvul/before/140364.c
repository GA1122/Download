VisibleSelection Editor::selectionForCommand(Event* event) {
  VisibleSelection selection =
      frame().selection().computeVisibleSelectionInDOMTreeDeprecated();
  if (!event)
    return selection;
  TextControlElement* textControlOfSelectionStart =
      enclosingTextControl(selection.start());
  TextControlElement* textControlOfTarget =
      isTextControlElement(*event->target()->toNode())
          ? toTextControlElement(event->target()->toNode())
          : nullptr;
  if (textControlOfTarget &&
      (selection.start().isNull() ||
       textControlOfTarget != textControlOfSelectionStart)) {
    if (Range* range = textControlOfTarget->selection()) {
      return createVisibleSelection(
          SelectionInDOMTree::Builder()
              .setBaseAndExtent(EphemeralRange(range))
              .setIsDirectional(selection.isDirectional())
              .build());
    }
  }
  return selection;
}
