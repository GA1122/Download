static void report_usage(FILE *out, int count, size_t bytes,
						 backtrace_t *bt, bool detailed)
{
	fprintf(out, "%zu bytes total, %d allocations, %zu bytes average:\n",
			bytes, count, bytes / count);
	bt->log(bt, out, detailed);
}
