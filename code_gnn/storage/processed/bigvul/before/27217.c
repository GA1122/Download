SYSCALL_DEFINE2(osf_proplist_syscall, enum pl_code, code,
		union pl_args __user *, args)
{
	long error;
	int __user *min_buf_size_ptr;

	switch (code) {
	case PL_SET:
		if (get_user(error, &args->set.nbytes))
			error = -EFAULT;
		break;
	case PL_FSET:
		if (get_user(error, &args->fset.nbytes))
			error = -EFAULT;
		break;
	case PL_GET:
		error = get_user(min_buf_size_ptr, &args->get.min_buf_size);
		if (error)
			break;
		error = put_user(0, min_buf_size_ptr);
		break;
	case PL_FGET:
		error = get_user(min_buf_size_ptr, &args->fget.min_buf_size);
		if (error)
			break;
		error = put_user(0, min_buf_size_ptr);
		break;
	case PL_DEL:
	case PL_FDEL:
		error = 0;
		break;
	default:
		error = -EOPNOTSUPP;
		break;
	};
	return error;
}
