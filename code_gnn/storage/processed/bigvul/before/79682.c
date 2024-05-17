R_API ut64 r_bin_java_bootstrap_method_calc_size(RBinJavaBootStrapMethod *bsm) {
	RListIter *iter, *iter_tmp;
	RBinJavaBootStrapArgument *bsm_arg = NULL;
	ut64 size = 0;
	if (bsm) {
		size += 6;
		size += 2;
		size += 2;
		r_list_foreach_safe (bsm->bootstrap_arguments, iter, iter_tmp, bsm_arg) {
			if (bsm_arg) {
				size += r_bin_java_bootstrap_arg_calc_size (bsm_arg);
			} else {
			}
		}
	}
	return size;
}
