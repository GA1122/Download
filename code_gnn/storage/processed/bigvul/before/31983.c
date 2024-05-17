inline void perf_swevent_put_recursion_context(int rctx)
{
	struct swevent_htable *swhash = &__get_cpu_var(swevent_htable);

	put_recursion_context(swhash->recursion, rctx);
}