float AudioParam::value()
{
    if (context() && context()->isAudioThread()) {
        bool hasValue;
        float timelineValue = m_timeline.valueForContextTime(context(), narrowPrecisionToFloat(m_value), hasValue);

        if (hasValue)
            m_value = timelineValue;
    }

    return narrowPrecisionToFloat(m_value);
}
