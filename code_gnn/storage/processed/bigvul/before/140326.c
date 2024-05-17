Element* Editor::findEventTargetFromSelection() const {
  return findEventTargetFrom(
      frame().selection().computeVisibleSelectionInDOMTreeDeprecated());
}
