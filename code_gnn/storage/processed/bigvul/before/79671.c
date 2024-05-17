R_API ut64 r_bin_java_annotation_calc_size(RBinJavaAnnotation *annotation) {
	ut64 sz = 0;
	RListIter *iter, *iter_tmp;
	RBinJavaElementValuePair *ev_pairs = NULL;
	if (!annotation) {
		return sz;
	}
	sz += 2;
	sz += 2;
	r_list_foreach_safe (annotation->element_value_pairs, iter, iter_tmp, ev_pairs) {
		if (ev_pairs) {
			sz += r_bin_java_element_pair_calc_size (ev_pairs);
		}
	}
	return sz;
}