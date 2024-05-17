static size_t snd_compr_calc_avail(struct snd_compr_stream *stream,
		struct snd_compr_avail *avail)
{
	long avail_calc;  

	snd_compr_update_tstamp(stream, &avail->tstamp);

	 

	if (stream->runtime->total_bytes_available == 0 &&
			stream->runtime->state == SNDRV_PCM_STATE_SETUP) {
		pr_debug("detected init and someone forgot to do a write\n");
		return stream->runtime->buffer_size;
	}
	pr_debug("app wrote %lld, DSP consumed %lld\n",
			stream->runtime->total_bytes_available,
			stream->runtime->total_bytes_transferred);
	if (stream->runtime->total_bytes_available ==
				stream->runtime->total_bytes_transferred) {
		pr_debug("both pointers are same, returning full avail\n");
		return stream->runtime->buffer_size;
	}

	 

	avail_calc = stream->runtime->buffer_size -
		(stream->runtime->app_pointer - stream->runtime->hw_pointer);
	pr_debug("calc avail as %ld, app_ptr %lld, hw+ptr %lld\n", avail_calc,
			stream->runtime->app_pointer,
			stream->runtime->hw_pointer);
	if (avail_calc >= stream->runtime->buffer_size)
		avail_calc -= stream->runtime->buffer_size;
	pr_debug("ret avail as %ld\n", avail_calc);
	avail->avail = avail_calc;
	return avail_calc;
}
