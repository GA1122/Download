R_API ut64 r_bin_java_bootstrap_arg_calc_size(RBinJavaBootStrapArgument *bsm_arg) {
	ut64 size = 0;
	if (bsm_arg) {
		size += 2;
	}
	return size;
}
