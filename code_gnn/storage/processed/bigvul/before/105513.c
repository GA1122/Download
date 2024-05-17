static void webkit_web_view_set_property(GObject* object, guint prop_id, const GValue* value, GParamSpec *pspec)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(object);

    switch(prop_id) {
    case PROP_EDITABLE:
        webkit_web_view_set_editable(webView, g_value_get_boolean(value));
        break;
    case PROP_SETTINGS:
        webkit_web_view_set_settings(webView, WEBKIT_WEB_SETTINGS(g_value_get_object(value)));
        break;
    case PROP_WINDOW_FEATURES:
        webkit_web_view_set_window_features(webView, WEBKIT_WEB_WINDOW_FEATURES(g_value_get_object(value)));
        break;
    case PROP_TRANSPARENT:
        webkit_web_view_set_transparent(webView, g_value_get_boolean(value));
        break;
    case PROP_ZOOM_LEVEL:
        webkit_web_view_set_zoom_level(webView, g_value_get_float(value));
        break;
    case PROP_FULL_CONTENT_ZOOM:
        webkit_web_view_set_full_content_zoom(webView, g_value_get_boolean(value));
        break;
    case PROP_CUSTOM_ENCODING:
        webkit_web_view_set_custom_encoding(webView, g_value_get_string(value));
        break;
    case PROP_VIEW_MODE:
        webkit_web_view_set_view_mode(webView, static_cast<WebKitWebViewViewMode>(g_value_get_enum(value)));
        break;
#ifndef GTK_API_VERSION_2
    case PROP_HADJUSTMENT:
        setHorizontalAdjustment(webView, static_cast<GtkAdjustment*>(g_value_get_object(value)));
        break;
    case PROP_VADJUSTMENT:
        setVerticalAdjustment(webView, static_cast<GtkAdjustment*>(g_value_get_object(value)));
        break;
    case PROP_HSCROLL_POLICY:
        setHorizontalScrollPolicy(webView, static_cast<GtkScrollablePolicy>(g_value_get_enum(value)));
        break;
    case PROP_VSCROLL_POLICY:
        setVerticalScrollPolicy(webView, static_cast<GtkScrollablePolicy>(g_value_get_enum(value)));
        break;
#endif
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}
