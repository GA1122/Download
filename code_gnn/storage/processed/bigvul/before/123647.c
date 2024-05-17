PassOwnPtr<GeolocationInspectorAgent> GeolocationInspectorAgent::create(GeolocationController* controller)
{
    return adoptPtr(new GeolocationInspectorAgent(controller));
}
