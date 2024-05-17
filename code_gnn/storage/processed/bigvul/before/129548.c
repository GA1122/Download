bool AudioParam::smooth()
{
    bool useTimelineValue = false;
    if (context())
        m_value = m_timeline.valueForContextTime(context(), narrowPrecisionToFloat(m_value), useTimelineValue);

    if (m_smoothedValue == m_value) {
        return true;
    }

    if (useTimelineValue)
        m_smoothedValue = m_value;
    else {
        m_smoothedValue += (m_value - m_smoothedValue) * DefaultSmoothingConstant;

        if (fabs(m_smoothedValue - m_value) < SnapThreshold)  
            m_smoothedValue = m_value;
    }

    return false;
}
