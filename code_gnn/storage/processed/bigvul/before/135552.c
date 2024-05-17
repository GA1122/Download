Element* Editor::FindEventTargetFromSelection() const {
  return FindEventTargetFrom(
      GetFrame().Selection().ComputeVisibleSelectionInDOMTreeDeprecated());
}
