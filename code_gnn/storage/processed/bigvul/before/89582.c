newSWFInput_stream(FILE* f)
{
	SWFInput input = (SWFInput)malloc(sizeof(struct SWFInput_s));
	struct SWFInputStreamData *data;

	 
	if (NULL == input)
		return NULL;

	data = (struct SWFInputStreamData *)malloc(sizeof(struct SWFInputStreamData));

	 
	if (NULL == data)
	{
		free(input);
		return NULL;
	}

	input->getChar = SWFInput_stream_getChar;
	input->destroy = SWFInput_stream_dtor;
	input->eof = SWFInput_stream_eof;
	input->read = SWFInput_stream_read;
	input->seek = SWFInput_stream_seek;

	input->offset = 0;
	input->length = 0;
	input->buffer = 0;
	input->bufbits = 0;

	data->file = f;
	data->buffer = NULL;

	input->data = (void *)data;

#if TRACK_ALLOCS
	input->gcnode = ming_gc_add_node(input, (dtorfunctype) destroySWFInput);
#endif

	return input;
}
