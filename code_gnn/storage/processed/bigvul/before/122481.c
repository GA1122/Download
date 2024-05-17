void InspectorController::requestPageScaleFactor(float scale, const IntPoint& origin)
{
    m_inspectorClient->requestPageScaleFactor(scale, origin);
}
