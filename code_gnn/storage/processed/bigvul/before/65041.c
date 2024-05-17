static int check_map_prog_compatibility(struct bpf_map *map,
					struct bpf_prog *prog)

{
	 
	if (prog->type == BPF_PROG_TYPE_PERF_EVENT) {
		if (!check_map_prealloc(map)) {
			verbose("perf_event programs can only use preallocated hash map\n");
			return -EINVAL;
		}
		if (map->inner_map_meta &&
		    !check_map_prealloc(map->inner_map_meta)) {
			verbose("perf_event programs can only use preallocated inner hash map\n");
			return -EINVAL;
		}
	}
	return 0;
}
