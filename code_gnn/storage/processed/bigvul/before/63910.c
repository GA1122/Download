 static bool timerfd_canceled(struct timerfd_ctx *ctx)
 {
 	if (!ctx->might_cancel || ctx->moffs != KTIME_MAX)
		return false;
	ctx->moffs = ktime_mono_to_real(0);
	return true;
}