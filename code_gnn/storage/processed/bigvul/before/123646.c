void GeolocationInspectorAgent::clearGeolocationOverride(ErrorString*)
{
    if (!m_geolocationOverridden)
        return;
    m_geolocationOverridden = false;
    m_geolocationPosition.clear();

    if (m_platformGeolocationPosition.get())
        m_controller->positionChanged(m_platformGeolocationPosition.get());
}
