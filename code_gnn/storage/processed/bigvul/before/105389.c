static gboolean webkit_navigation_request_handled(GSignalInvocationHint* ihint, GValue* returnAccu, const GValue* handlerReturn, gpointer dummy)
{
    WebKitNavigationResponse navigationResponse = (WebKitNavigationResponse)g_value_get_enum(handlerReturn);
    g_value_set_enum(returnAccu, navigationResponse);

    if (navigationResponse != WEBKIT_NAVIGATION_RESPONSE_ACCEPT)
        return FALSE;

    return TRUE;
}
