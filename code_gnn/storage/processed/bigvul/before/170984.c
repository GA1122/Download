void AudioFlinger::EffectChain::clearInputBuffer_l(sp<ThreadBase> thread)
{
 const size_t frameSize =
            audio_bytes_per_sample(AUDIO_FORMAT_PCM_16_BIT) * min(FCC_2, thread->channelCount());
    memset(mInBuffer, 0, thread->frameCount() * frameSize);
}
