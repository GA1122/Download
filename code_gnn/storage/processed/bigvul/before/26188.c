put_callchain_entry(int rctx)
{
	put_recursion_context(__get_cpu_var(callchain_recursion), rctx);
}
