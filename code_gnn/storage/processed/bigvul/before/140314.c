static VisibleSelection correctedVisibleSelection(
    const VisibleSelection& passedSelection) {
  if (!passedSelection.base().isConnected() ||
      !passedSelection.extent().isConnected())
    return VisibleSelection();
  DCHECK(!passedSelection.base().document()->needsLayoutTreeUpdate());
  return createVisibleSelection(passedSelection.asSelection());
}
