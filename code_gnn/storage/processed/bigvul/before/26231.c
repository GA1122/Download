void perf_output_copy(struct perf_output_handle *handle,
		      const void *buf, unsigned int len)
{
	__output_copy(handle, buf, len);
}
