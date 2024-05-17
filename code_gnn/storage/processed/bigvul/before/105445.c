static void webkit_web_view_get_property(GObject* object, guint prop_id, GValue* value, GParamSpec* pspec)
{
    WebKitWebView* webView = WEBKIT_WEB_VIEW(object);

    switch(prop_id) {
    case PROP_TITLE:
        g_value_set_string(value, webkit_web_view_get_title(webView));
        break;
    case PROP_URI:
        g_value_set_string(value, webkit_web_view_get_uri(webView));
        break;
    case PROP_COPY_TARGET_LIST:
        g_value_set_boxed(value, webkit_web_view_get_copy_target_list(webView));
        break;
    case PROP_PASTE_TARGET_LIST:
        g_value_set_boxed(value, webkit_web_view_get_paste_target_list(webView));
        break;
    case PROP_EDITABLE:
        g_value_set_boolean(value, webkit_web_view_get_editable(webView));
        break;
    case PROP_SETTINGS:
        g_value_set_object(value, webkit_web_view_get_settings(webView));
        break;
    case PROP_WEB_INSPECTOR:
        g_value_set_object(value, webkit_web_view_get_inspector(webView));
        break;
    case PROP_VIEWPORT_ATTRIBUTES:
        g_value_set_object(value, webkit_web_view_get_viewport_attributes(webView));
        break;
    case PROP_WINDOW_FEATURES:
        g_value_set_object(value, webkit_web_view_get_window_features(webView));
        break;
    case PROP_TRANSPARENT:
        g_value_set_boolean(value, webkit_web_view_get_transparent(webView));
        break;
    case PROP_ZOOM_LEVEL:
        g_value_set_float(value, webkit_web_view_get_zoom_level(webView));
        break;
    case PROP_FULL_CONTENT_ZOOM:
        g_value_set_boolean(value, webkit_web_view_get_full_content_zoom(webView));
        break;
    case PROP_ENCODING:
        g_value_set_string(value, webkit_web_view_get_encoding(webView));
        break;
    case PROP_CUSTOM_ENCODING:
        g_value_set_string(value, webkit_web_view_get_custom_encoding(webView));
        break;
    case PROP_LOAD_STATUS:
        g_value_set_enum(value, webkit_web_view_get_load_status(webView));
        break;
    case PROP_PROGRESS:
        g_value_set_double(value, webkit_web_view_get_progress(webView));
        break;
    case PROP_ICON_URI:
        g_value_set_string(value, webkit_web_view_get_icon_uri(webView));
        break;
    case PROP_IM_CONTEXT:
        g_value_set_object(value, webkit_web_view_get_im_context(webView));
        break;
    case PROP_VIEW_MODE:
        g_value_set_enum(value, webkit_web_view_get_view_mode(webView));
        break;
#ifndef GTK_API_VERSION_2
    case PROP_HADJUSTMENT:
        g_value_set_object(value, getHorizontalAdjustment(webView));
        break;
    case PROP_VADJUSTMENT:
        g_value_set_object(value, getVerticalAdjustment(webView));
        break;
    case PROP_HSCROLL_POLICY:
        g_value_set_enum(value, getHorizontalScrollPolicy(webView));
        break;
    case PROP_VSCROLL_POLICY:
        g_value_set_enum(value, getVerticalScrollPolicy(webView));
        break;
#endif
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
    }
}
