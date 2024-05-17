u32 gf_isom_oinf_size_entry(void *entry)
{
	GF_OperatingPointsInformation* ptr = (GF_OperatingPointsInformation *)entry;
	u32 size = 0, i ,j, count;
	if (!ptr) return 0;

	size += 3;  
	count=gf_list_count(ptr->profile_tier_levels);
	size += count * 12;  
	size += 2; 
	count=gf_list_count(ptr->operating_points);
	for (i = 0; i < count; i++) {
		LHEVC_OperatingPoint *op = (LHEVC_OperatingPoint *)gf_list_get(ptr->operating_points, i);;
		size += 2  + 1  + 1 ;
		size += op->layer_count * 2;
		size += 9;
		if (op->frame_rate_info_flag) {
			size += 3;
		}
		if (op->bit_rate_info_flag) {
			size += 8;
		}
	}
	size += 1; 
	count=gf_list_count(ptr->dependency_layers);
	for (i = 0; i < count; i++) {
		LHEVC_DependentLayer *dep = (LHEVC_DependentLayer *)gf_list_get(ptr->dependency_layers, i);
		size += 1  + 1 ;
		size += dep->num_layers_dependent_on * 1; 
		for (j = 0; j < 16; j++) {
			if (ptr->scalability_mask & (1 << j))
				size += 1; 
		}
	}
	return size;
}
