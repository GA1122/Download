void TestController::decidePolicyForNotificationPermissionRequest(WKPageRef page, WKSecurityOriginRef origin, WKNotificationPermissionRequestRef request, const void*)
{
    TestController::shared().decidePolicyForNotificationPermissionRequest(page, origin, request);
}
