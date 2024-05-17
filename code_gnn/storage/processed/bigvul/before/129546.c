void AudioParam::disconnect(AudioNodeOutput* output)
{
    ASSERT(context()->isGraphOwner());

    ASSERT(output);
    if (!output)
        return;

    if (m_outputs.contains(output)) {
        m_outputs.remove(output);
        changedOutputs();
        output->removeParam(this);
    }
}
