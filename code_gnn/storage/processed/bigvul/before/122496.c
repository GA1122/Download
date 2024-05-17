void InspectorResourceAgent::didRecalculateStyle(int)
{
    m_isRecalculatingStyle = false;
    m_styleRecalculationInitiator = nullptr;
}
