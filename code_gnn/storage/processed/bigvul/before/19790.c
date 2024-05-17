i915_gem_execbuffer_reserve(struct intel_ring_buffer *ring,
			    struct drm_file *file,
			    struct list_head *objects)
{
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
	struct drm_i915_gem_object *obj;
	int ret, retry;
	bool has_fenced_gpu_access = INTEL_INFO(ring->dev)->gen < 4;
	struct list_head ordered_objects;

	INIT_LIST_HEAD(&ordered_objects);
	while (!list_empty(objects)) {
		struct drm_i915_gem_exec_object2 *entry;
		bool need_fence, need_mappable;

		obj = list_first_entry(objects,
				       struct drm_i915_gem_object,
				       exec_list);
		entry = obj->exec_entry;

		need_fence =
			has_fenced_gpu_access &&
			entry->flags & EXEC_OBJECT_NEEDS_FENCE &&
			obj->tiling_mode != I915_TILING_NONE;
		need_mappable =
			entry->relocation_count ? true : need_fence;

		if (need_mappable)
			list_move(&obj->exec_list, &ordered_objects);
		else
			list_move_tail(&obj->exec_list, &ordered_objects);

		obj->base.pending_read_domains = 0;
		obj->base.pending_write_domain = 0;
	}
	list_splice(&ordered_objects, objects);

	 
	retry = 0;
	do {
		ret = 0;

		 
		list_for_each_entry(obj, objects, exec_list) {
			struct drm_i915_gem_exec_object2 *entry = obj->exec_entry;
			bool need_fence, need_mappable;

			if (!obj->gtt_space)
				continue;

			need_fence =
				has_fenced_gpu_access &&
				entry->flags & EXEC_OBJECT_NEEDS_FENCE &&
				obj->tiling_mode != I915_TILING_NONE;
			need_mappable =
				entry->relocation_count ? true : need_fence;

			if ((entry->alignment && obj->gtt_offset & (entry->alignment - 1)) ||
			    (need_mappable && !obj->map_and_fenceable))
				ret = i915_gem_object_unbind(obj);
			else
				ret = pin_and_fence_object(obj, ring);
			if (ret)
				goto err;
		}

		 
		list_for_each_entry(obj, objects, exec_list) {
			if (obj->gtt_space)
				continue;

			ret = pin_and_fence_object(obj, ring);
			if (ret) {
				int ret_ignore;

				 
				ret_ignore = i915_gem_object_unbind(obj);
				(void)ret_ignore;
				WARN_ON(obj->gtt_space);
				break;
			}
		}

		 
		list_for_each_entry(obj, objects, exec_list) {
			struct drm_i915_gem_exec_object2 *entry;

			if (!obj->gtt_space)
				continue;

			entry = obj->exec_entry;
			if (entry->flags & __EXEC_OBJECT_HAS_FENCE) {
				i915_gem_object_unpin_fence(obj);
				entry->flags &= ~__EXEC_OBJECT_HAS_FENCE;
			}

			i915_gem_object_unpin(obj);

			 
			if (dev_priv->mm.aliasing_ppgtt && !obj->has_aliasing_ppgtt_mapping) {
				i915_ppgtt_bind_object(dev_priv->mm.aliasing_ppgtt,
						       obj, obj->cache_level);

				obj->has_aliasing_ppgtt_mapping = 1;
			}
		}

		if (ret != -ENOSPC || retry > 1)
			return ret;

		 
		ret = i915_gem_evict_everything(ring->dev, retry == 0);
		if (ret)
			return ret;

		retry++;
	} while (1);

err:
	list_for_each_entry_continue_reverse(obj, objects, exec_list) {
		struct drm_i915_gem_exec_object2 *entry;

		if (!obj->gtt_space)
			continue;

		entry = obj->exec_entry;
		if (entry->flags & __EXEC_OBJECT_HAS_FENCE) {
			i915_gem_object_unpin_fence(obj);
			entry->flags &= ~__EXEC_OBJECT_HAS_FENCE;
		}

		i915_gem_object_unpin(obj);
	}

	return ret;
}
