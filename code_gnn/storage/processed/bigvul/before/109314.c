 GeolocationPosition* InspectorPageAgent::overrideGeolocationPosition(GeolocationPosition* position)
{
    if (m_geolocationOverridden) {
        if (position)
            m_platformGeolocationPosition = position;
        return m_geolocationPosition.get();
    }
    return position;
}
