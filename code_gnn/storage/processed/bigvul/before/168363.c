void BrowserView::ShowImeWarningBubble(
    const extensions::Extension* extension,
    const base::Callback<void(ImeWarningBubblePermissionStatus status)>&
        callback) {
  ImeWarningBubbleView::ShowBubble(extension, this, callback);
}
