void destroy_rate_histogram(struct rate_hist *hist) {
 if (hist) {
    free(hist->pts);
    free(hist->sz);
    free(hist);
 }
}
