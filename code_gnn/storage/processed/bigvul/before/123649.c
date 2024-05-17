void GeolocationInspectorAgent::setGeolocationOverride(ErrorString* error, const double* latitude, const double* longitude, const double* accuracy)
{
    GeolocationPosition* position = m_controller->lastPosition();
    if (!m_geolocationOverridden && position)
        m_platformGeolocationPosition = position;

    m_geolocationOverridden = true;
    if (latitude && longitude && accuracy)
        m_geolocationPosition = GeolocationPosition::create(currentTimeMS(), *latitude, *longitude, *accuracy);
    else
        m_geolocationPosition.clear();

    m_controller->positionChanged(0);  
}
