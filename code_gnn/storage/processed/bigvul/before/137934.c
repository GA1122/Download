double AXLayoutObject::estimatedLoadingProgress() const {
  if (!m_layoutObject)
    return 0;

  if (isLoaded())
    return 1.0;

  if (LocalFrame* frame = m_layoutObject->document().frame())
    return frame->loader().progress().estimatedProgress();
  return 0;
}
