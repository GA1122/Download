TEE_Result tee_mmu_user_pa2va_helper(const struct user_ta_ctx *utc,
				      paddr_t pa, void **va)
{
	TEE_Result res;
	paddr_t p;
	struct vm_region *region;

	TAILQ_FOREACH(region, &utc->vm_info->regions, link) {
		size_t granule;
		size_t size;
		size_t ofs;

		 
		if (!region->mobj)
			continue;

		 
		granule = region->mobj->phys_granule;
		assert(!granule || IS_POWER_OF_TWO(granule));

		for (ofs = region->offset; ofs < region->size; ofs += size) {

			if (granule) {
				 
				size = granule - (ofs & (granule - 1));

				if (size > (region->size - ofs))
					size = region->size - ofs;
			} else
				size = region->size;

			res = mobj_get_pa(region->mobj, ofs, granule, &p);
			if (res != TEE_SUCCESS)
				return res;

			if (core_is_buffer_inside(pa, 1, p, size)) {
				 
				ofs -= region->offset;
				 
				p = pa - p;

				*va = (void *)(region->va + ofs + (vaddr_t)p);
				return TEE_SUCCESS;
			}
		}
	}

	return TEE_ERROR_ACCESS_DENIED;
}
