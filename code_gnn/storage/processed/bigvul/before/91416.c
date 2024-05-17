static bool check_refcount_ok(const struct bpf_func_proto *fn)
{
	int count = 0;

	if (arg_type_is_refcounted(fn->arg1_type))
		count++;
	if (arg_type_is_refcounted(fn->arg2_type))
		count++;
	if (arg_type_is_refcounted(fn->arg3_type))
		count++;
	if (arg_type_is_refcounted(fn->arg4_type))
		count++;
	if (arg_type_is_refcounted(fn->arg5_type))
		count++;

	 
	return count <= 1;
}