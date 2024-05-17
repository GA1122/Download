void WebPluginDelegateProxy::PaintSadPlugin(WebKit::WebCanvas* native_context,
                                            const gfx::Rect& rect) {
  if (!sad_plugin_)
    sad_plugin_ = content::GetContentClient()->renderer()->GetSadPluginBitmap();
  if (sad_plugin_)
    webkit::PaintSadPlugin(native_context, plugin_rect_, *sad_plugin_);
}
