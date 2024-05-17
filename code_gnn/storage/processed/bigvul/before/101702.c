bool Browser::NavigateToIndexWithDisposition(int index,
                                             WindowOpenDisposition disp) {
  NavigationController& controller =
      GetOrCloneTabForDisposition(disp)->controller();
  if (index < 0 || index >= controller.entry_count())
    return false;
  controller.GoToIndex(index);
  return true;
}
