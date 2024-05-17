void OxideQQuickWebViewPrivate::OnEditingCapabilitiesChanged() {
  Q_Q(OxideQQuickWebView);

  emit q->editingCapabilitiesChanged();
}
