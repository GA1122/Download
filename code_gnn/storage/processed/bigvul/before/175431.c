static void apply_dsp(struct pcm_device *iodev, uint8_t *buf, size_t frames)
{
 struct cras_dsp_context *ctx;
 struct pipeline *pipeline;

	ctx = iodev->dsp_context;
 if (!ctx)
 return;

	pipeline = cras_dsp_get_pipeline(ctx);
 if (!pipeline)
 return;

	cras_dsp_pipeline_apply(pipeline,
				buf,
				frames);

	cras_dsp_put_pipeline(ctx);
}
