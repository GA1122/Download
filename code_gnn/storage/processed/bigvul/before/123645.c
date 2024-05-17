GeolocationInspectorAgent::GeolocationInspectorAgent(GeolocationController* controller)
    : InspectorBaseAgent<GeolocationInspectorAgent>("Geolocation")
    , m_controller(controller)
    , m_geolocationOverridden(false)
{
}
