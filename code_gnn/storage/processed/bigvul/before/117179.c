    static gboolean openWindowCallback(WebKitWebInspector*, InspectorTest* test)
    {
        return test->openWindow();
    }
