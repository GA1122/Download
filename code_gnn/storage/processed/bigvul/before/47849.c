static void update_audio_tstamp(struct snd_pcm_substream *substream,
				struct timespec *curr_tstamp,
				struct timespec *audio_tstamp)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	u64 audio_frames, audio_nsecs;
	struct timespec driver_tstamp;

	if (runtime->tstamp_mode != SNDRV_PCM_TSTAMP_ENABLE)
		return;

	if (!(substream->ops->get_time_info) ||
		(runtime->audio_tstamp_report.actual_type ==
			SNDRV_PCM_AUDIO_TSTAMP_TYPE_DEFAULT)) {

		 

		audio_frames = runtime->hw_ptr_wrap + runtime->status->hw_ptr;

		if (runtime->audio_tstamp_config.report_delay) {
			if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
				audio_frames -=  runtime->delay;
			else
				audio_frames +=  runtime->delay;
		}
		audio_nsecs = div_u64(audio_frames * 1000000000LL,
				runtime->rate);
		*audio_tstamp = ns_to_timespec(audio_nsecs);
	}
	runtime->status->audio_tstamp = *audio_tstamp;
	runtime->status->tstamp = *curr_tstamp;

	 
	snd_pcm_gettime(substream->runtime, (struct timespec *)&driver_tstamp);
	runtime->driver_tstamp = driver_tstamp;
}
