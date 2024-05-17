void AudioParam::calculateFinalValues(float* values, unsigned numberOfValues, bool sampleAccurate)
{
    bool isGood = context() && context()->isAudioThread() && values && numberOfValues;
    ASSERT(isGood);
    if (!isGood)
        return;


    if (sampleAccurate) {
        calculateTimelineValues(values, numberOfValues);
    } else {
        bool hasValue;
        float timelineValue = m_timeline.valueForContextTime(context(), narrowPrecisionToFloat(m_value), hasValue);

        if (hasValue)
            m_value = timelineValue;

        values[0] = narrowPrecisionToFloat(m_value);
    }

    RefPtr<AudioBus> summingBus = AudioBus::create(1, numberOfValues, false);
    summingBus->setChannelMemory(0, values, numberOfValues);

    for (unsigned i = 0; i < numberOfRenderingConnections(); ++i) {
        AudioNodeOutput* output = renderingOutput(i);
        ASSERT(output);

        AudioBus* connectionBus = output->pull(0, AudioNode::ProcessingSizeInFrames);

        summingBus->sumFrom(*connectionBus);
    }
}
