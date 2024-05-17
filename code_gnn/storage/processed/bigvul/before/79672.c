R_API ut64 r_bin_java_annotation_default_attr_calc_size(RBinJavaAttrInfo *attr) {
	ut64 size = 0;
	if (attr) {
		size += 6;
		size += r_bin_java_element_value_calc_size (attr->info.annotation_default_attr.default_value);
	}
	return size;
}
