void beforeAll()
{
    g_setenv("WEBKIT_INSPECTOR_PATH", WEBKIT_INSPECTOR_PATH, FALSE);
    InspectorTest::add("WebKitWebInspector", "default", testInspectorDefault);
    CustomInspectorTest::add("WebKitWebInspector", "manual-attach-detach", testInspectorManualAttachDetach);
    CustomInspectorTest::add("WebKitWebInspector", "custom-container-destroyed", testInspectorCustomContainerDestroyed);
}
