struct rate_hist *init_rate_histogram(const vpx_codec_enc_cfg_t *cfg,
 const vpx_rational_t *fps) {
 int i;
 struct rate_hist *hist = malloc(sizeof(*hist));

  hist->samples = cfg->rc_buf_sz * 5 / 4 * fps->num / fps->den / 1000;

 if (hist->samples == 0)
    hist->samples = 1;

  hist->frames = 0;
  hist->total = 0;

  hist->pts = calloc(hist->samples, sizeof(*hist->pts));
  hist->sz = calloc(hist->samples, sizeof(*hist->sz));
 for (i = 0; i < RATE_BINS; i++) {
    hist->bucket[i].low = INT_MAX;
    hist->bucket[i].high = 0;
    hist->bucket[i].count = 0;
 }

 return hist;
}
