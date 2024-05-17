void TestController::unavailablePluginButtonClicked(WKPageRef, WKPluginUnavailabilityReason, WKStringRef, WKStringRef, WKStringRef, const void*)
{
    printf("MISSING PLUGIN BUTTON PRESSED\n");
}
