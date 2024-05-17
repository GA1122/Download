static int snd_compr_next_track(struct snd_compr_stream *stream)
{
	int retval;

	 
	if (stream->runtime->state != SNDRV_PCM_STATE_RUNNING)
		return -EPERM;

	 
	if (stream->metadata_set == false)
		return -EPERM;

	retval = stream->ops->trigger(stream, SND_COMPR_TRIGGER_NEXT_TRACK);
	if (retval != 0)
		return retval;
	stream->metadata_set = false;
	stream->next_track = true;
	return 0;
}
