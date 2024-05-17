R_API void r_bin_java_annotation_array_free(void   *a) {
	RBinJavaAnnotationsArray *annotation_array = a;
	RListIter *iter = NULL, *iter_tmp = NULL;
	RBinJavaAnnotation *annotation;
	if (annotation_array->annotations == NULL) {
		return;
	}
	r_list_foreach_safe (annotation_array->annotations, iter, iter_tmp, annotation) {
		if (annotation) {
			r_bin_java_annotation_free (annotation);
		}
	}
	r_list_free (annotation_array->annotations);
	free (annotation_array);
}
