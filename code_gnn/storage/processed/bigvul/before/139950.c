bool computeLockedPendingUserGesture(Document& document) {
  if (!document.settings())
    return false;

  if (document.settings()->getCrossOriginMediaPlaybackRequiresUserGesture() &&
      isDocumentCrossOrigin(document)) {
    return true;
  }

  return document.settings()->getMediaPlaybackRequiresUserGesture();
}
