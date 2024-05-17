void WebPageProxy::requestGeolocationPermissionForFrame(uint64_t geolocationID, uint64_t frameID, String originIdentifier)
{
    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    RefPtr<WebSecurityOrigin> origin = WebSecurityOrigin::create(originIdentifier);
    RefPtr<GeolocationPermissionRequestProxy> request = m_geolocationPermissionRequestManager.createRequest(geolocationID);

    if (!m_uiClient.decidePolicyForGeolocationPermissionRequest(this, frame, origin.get(), request.get()))
        request->deny();
}
