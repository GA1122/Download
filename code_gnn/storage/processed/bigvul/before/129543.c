void AudioParam::calculateSampleAccurateValues(float* values, unsigned numberOfValues)
{
    bool isSafe = context() && context()->isAudioThread() && values && numberOfValues;
    ASSERT(isSafe);
    if (!isSafe)
        return;

    calculateFinalValues(values, numberOfValues, true);
}
