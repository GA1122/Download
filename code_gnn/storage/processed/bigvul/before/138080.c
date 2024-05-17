bool AXNodeObject::isNonNativeTextControl() const {
  if (isNativeTextControl())
    return false;

  if (hasContentEditableAttributeSet())
    return true;

  if (isARIATextControl())
    return true;

  return false;
}
