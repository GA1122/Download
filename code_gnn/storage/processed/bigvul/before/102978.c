bool TabStripModel::IsContextMenuCommandChecked(
    int context_index,
    ContextMenuCommand command_id) const {
  switch (command_id) {
    case CommandUseVerticalTabs:
      return delegate()->UseVerticalTabs();
    default:
      NOTREACHED();
      break;
  }
  return false;
}
