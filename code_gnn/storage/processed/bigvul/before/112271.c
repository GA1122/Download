void ChromeRenderMessageFilter::OnCanTriggerClipboardWrite(
    const GURL& origin, bool* allowed) {
  *allowed = (origin.SchemeIs(chrome::kExtensionScheme) ||
      extension_info_map_->SecurityOriginHasAPIPermission(
          origin, render_process_id_, ExtensionAPIPermission::kClipboardWrite));
}
