static void testInspectorCustomContainerDestroyed(CustomInspectorTest* test, gconstpointer)
{
    test->showInWindowAndWaitUntilMapped(GTK_WINDOW_TOPLEVEL);
    test->resizeView(200, 200);
    test->loadHtml("<html><body><p>WebKitGTK+ Inspector test</p></body></html>", 0);
    test->waitUntilLoadFinished();

    test->showAndWaitUntilFinished(false);
    test->assertObjectIsDeletedWhenTestFinishes(G_OBJECT(webkit_web_inspector_get_web_view(test->m_inspector)));
    g_assert(!webkit_web_inspector_is_attached(test->m_inspector));

    test->m_events.clear();
    test->destroyWindowAndWaitUntilClosed();
    Vector<InspectorTest::InspectorEvents>& events = test->m_events;
    g_assert_cmpint(events.size(), ==, 1);
    g_assert_cmpint(events[0], ==, InspectorTest::Closed);
    test->m_events.clear();
}
