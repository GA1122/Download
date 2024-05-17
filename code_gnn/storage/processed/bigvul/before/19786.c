i915_gem_execbuffer_relocate_entry(struct drm_i915_gem_object *obj,
				   struct eb_objects *eb,
				   struct drm_i915_gem_relocation_entry *reloc)
{
	struct drm_device *dev = obj->base.dev;
	struct drm_gem_object *target_obj;
	uint32_t target_offset;
	int ret = -EINVAL;

	 
	target_obj = &eb_get_object(eb, reloc->target_handle)->base;
	if (unlikely(target_obj == NULL))
		return -ENOENT;

	target_offset = to_intel_bo(target_obj)->gtt_offset;

	 
	if (unlikely(target_offset == 0)) {
		DRM_DEBUG("No GTT space found for object %d\n",
			  reloc->target_handle);
		return ret;
	}

	 
	if (unlikely(reloc->write_domain & (reloc->write_domain - 1))) {
		DRM_DEBUG("reloc with multiple write domains: "
			  "obj %p target %d offset %d "
			  "read %08x write %08x",
			  obj, reloc->target_handle,
			  (int) reloc->offset,
			  reloc->read_domains,
			  reloc->write_domain);
		return ret;
	}
	if (unlikely((reloc->write_domain | reloc->read_domains)
		     & ~I915_GEM_GPU_DOMAINS)) {
		DRM_DEBUG("reloc with read/write non-GPU domains: "
			  "obj %p target %d offset %d "
			  "read %08x write %08x",
			  obj, reloc->target_handle,
			  (int) reloc->offset,
			  reloc->read_domains,
			  reloc->write_domain);
		return ret;
	}
	if (unlikely(reloc->write_domain && target_obj->pending_write_domain &&
		     reloc->write_domain != target_obj->pending_write_domain)) {
		DRM_DEBUG("Write domain conflict: "
			  "obj %p target %d offset %d "
			  "new %08x old %08x\n",
			  obj, reloc->target_handle,
			  (int) reloc->offset,
			  reloc->write_domain,
			  target_obj->pending_write_domain);
		return ret;
	}

	target_obj->pending_read_domains |= reloc->read_domains;
	target_obj->pending_write_domain |= reloc->write_domain;

	 
	if (target_offset == reloc->presumed_offset)
		return 0;

	 
	if (unlikely(reloc->offset > obj->base.size - 4)) {
		DRM_DEBUG("Relocation beyond object bounds: "
			  "obj %p target %d offset %d size %d.\n",
			  obj, reloc->target_handle,
			  (int) reloc->offset,
			  (int) obj->base.size);
		return ret;
	}
	if (unlikely(reloc->offset & 3)) {
		DRM_DEBUG("Relocation not 4-byte aligned: "
			  "obj %p target %d offset %d.\n",
			  obj, reloc->target_handle,
			  (int) reloc->offset);
		return ret;
	}

	reloc->delta += target_offset;
	if (obj->base.write_domain == I915_GEM_DOMAIN_CPU) {
		uint32_t page_offset = reloc->offset & ~PAGE_MASK;
		char *vaddr;

		vaddr = kmap_atomic(obj->pages[reloc->offset >> PAGE_SHIFT]);
		*(uint32_t *)(vaddr + page_offset) = reloc->delta;
		kunmap_atomic(vaddr);
	} else {
		struct drm_i915_private *dev_priv = dev->dev_private;
		uint32_t __iomem *reloc_entry;
		void __iomem *reloc_page;

		 
		if (obj->active && in_atomic())
			return -EFAULT;

		ret = i915_gem_object_set_to_gtt_domain(obj, 1);
		if (ret)
			return ret;

		 
		reloc->offset += obj->gtt_offset;
		reloc_page = io_mapping_map_atomic_wc(dev_priv->mm.gtt_mapping,
						      reloc->offset & PAGE_MASK);
		reloc_entry = (uint32_t __iomem *)
			(reloc_page + (reloc->offset & ~PAGE_MASK));
		iowrite32(reloc->delta, reloc_entry);
		io_mapping_unmap_atomic(reloc_page);
	}

	 
	reloc->presumed_offset = target_offset;

	return 0;
}