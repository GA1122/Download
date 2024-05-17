static toff_t tiff_seekproc(thandle_t clientdata, toff_t offset, int from)
{
	tiff_handle *th = (tiff_handle *)clientdata;
	gdIOCtx *ctx = th->ctx;
	int result;

	switch(from) {
	default:
	case SEEK_SET:
		 
		break;

	case SEEK_END:
		 
		offset = th->size + offset;
		break;

	case SEEK_CUR:
		 
		offset += th->pos;
		break;
	}

	 
	if((result = (ctx->seek)(ctx, offset))) {
		th->pos = offset;
	}

	return result ? offset : (toff_t)-1;
}
