 void InspectorClientImpl::overrideDeviceMetrics(int width, int height, float deviceScaleFactor, bool emulateViewport, bool fitWindow)
 {
     if (WebDevToolsAgentImpl* agent = devToolsAgent())
        agent->overrideDeviceMetrics(width, height, deviceScaleFactor, emulateViewport, fitWindow);
}
