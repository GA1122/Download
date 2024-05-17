PassRefPtr<Widget> FrameLoaderClient::createPlugin(const IntSize& pluginSize, HTMLPlugInElement* element, const KURL& url, const Vector<String>& paramNames, const Vector<String>& paramValues, const String& mimeType, bool loadManually)
{
     
    CString urlString = url.string().utf8();
    CString mimeTypeString = mimeType.utf8();

    ASSERT(paramNames.size() == paramValues.size());
    GOwnPtr<GHashTable> hash(g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free));
    for (unsigned i = 0; i < paramNames.size(); ++i) {
        g_hash_table_insert(hash.get(),
                            g_strdup(paramNames[i].utf8().data()),
                            g_strdup(paramValues[i].utf8().data()));
    }

    GtkWidget* gtkWidget = 0;
    g_signal_emit_by_name(getViewFromFrame(m_frame), "create-plugin-widget",
                          mimeTypeString.data(), urlString.data(), hash.get(), &gtkWidget);
    if (gtkWidget)
        return adoptRef(new GtkPluginWidget(gtkWidget));

    RefPtr<PluginView> pluginView = PluginView::create(core(m_frame), pluginSize, element, url, paramNames, paramValues, mimeType, loadManually);

    if (pluginView->status() == PluginStatusLoadedSuccessfully)
        return pluginView;

    return 0;
}
