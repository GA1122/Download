void AudioParam::calculateTimelineValues(float* values, unsigned numberOfValues)
{
    double sampleRate = context()->sampleRate();
    double startTime = context()->currentTime();
    double endTime = startTime + numberOfValues / sampleRate;

    m_value = m_timeline.valuesForTimeRange(startTime, endTime, narrowPrecisionToFloat(m_value), values, numberOfValues, sampleRate, sampleRate);
}
