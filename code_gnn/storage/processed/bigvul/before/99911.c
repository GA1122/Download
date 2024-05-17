void NPN_InvalidateRect(NPP id, NPRect *invalidRect) {



  scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
  DCHECK(plugin.get() != NULL);
  if (plugin.get() && plugin->webplugin()) {
    if (invalidRect) {
#if defined(OS_WIN)
      if (!plugin->windowless()) {
        RECT rect = {0};
        rect.left = invalidRect->left;
        rect.right = invalidRect->right;
        rect.top = invalidRect->top;
        rect.bottom = invalidRect->bottom;
        ::InvalidateRect(plugin->window_handle(), &rect, FALSE);
        return;
      }
#endif
      gfx::Rect rect(invalidRect->left,
                     invalidRect->top,
                     invalidRect->right - invalidRect->left,
                     invalidRect->bottom - invalidRect->top);
      plugin->webplugin()->InvalidateRect(rect);
    } else {
      plugin->webplugin()->Invalidate();
    }
  }
}
