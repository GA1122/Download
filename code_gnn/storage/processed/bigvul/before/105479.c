static gboolean webkit_web_view_query_tooltip(GtkWidget *widget, gint x, gint y, gboolean keyboard_mode, GtkTooltip *tooltip)
{
    WebKitWebViewPrivate* priv = WEBKIT_WEB_VIEW(widget)->priv;

    if (keyboard_mode) {
        WebKitWebView* webView = WEBKIT_WEB_VIEW(widget);

        Frame* coreFrame = core(webView)->focusController()->focusedOrMainFrame();
        if (!coreFrame)
            return FALSE;

        Node* node = getFocusedNode(coreFrame);
        if (!node)
            return FALSE;

        for (Node* titleNode = node; titleNode; titleNode = titleNode->parentNode()) {
            if (titleNode->isElementNode()) {
                String title = static_cast<Element*>(titleNode)->title();
                if (!title.isEmpty()) {
                    if (FrameView* view = coreFrame->view()) {
                        GdkRectangle area = view->contentsToWindow(node->getRect());
                        gtk_tooltip_set_tip_area(tooltip, &area);
                    }
                    gtk_tooltip_set_text(tooltip, title.utf8().data());

                    return TRUE;
                }
            }
        }

        return FALSE;
    }

    if (priv->tooltipText.length() > 0) {
        if (!keyboard_mode) {
            if (!priv->tooltipArea.isEmpty()) {
                GdkRectangle area = priv->tooltipArea;
                gtk_tooltip_set_tip_area(tooltip, &area);
            } else
                gtk_tooltip_set_tip_area(tooltip, 0);
        }
        gtk_tooltip_set_text(tooltip, priv->tooltipText.data());
        return TRUE;
    }

    return FALSE;
}
