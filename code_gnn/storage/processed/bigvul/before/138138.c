bool AXObject::computeIsInertOrAriaHidden(
    IgnoredReasons* ignoredReasons) const {
  if (getNode()) {
    if (getNode()->isInert()) {
      if (ignoredReasons) {
        HTMLDialogElement* dialog = getActiveDialogElement(getNode());
        if (dialog) {
          AXObject* dialogObject = axObjectCache().getOrCreate(dialog);
          if (dialogObject)
            ignoredReasons->push_back(
                IgnoredReason(AXActiveModalDialog, dialogObject));
          else
            ignoredReasons->push_back(IgnoredReason(AXInert));
        } else {
          ignoredReasons->push_back(IgnoredReason(AXInert));
        }
      }
      return true;
    }
  } else {
    AXObject* parent = parentObject();
    if (parent && parent->isInertOrAriaHidden()) {
      if (ignoredReasons)
        parent->computeIsInertOrAriaHidden(ignoredReasons);
      return true;
    }
  }

  const AXObject* hiddenRoot = ariaHiddenRoot();
  if (hiddenRoot) {
    if (ignoredReasons) {
      if (hiddenRoot == this)
        ignoredReasons->push_back(IgnoredReason(AXAriaHidden));
      else
        ignoredReasons->push_back(IgnoredReason(AXAriaHiddenRoot, hiddenRoot));
    }
    return true;
  }

  return false;
}
