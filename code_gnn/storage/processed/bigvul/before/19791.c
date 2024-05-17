i915_gem_execbuffer_retire_commands(struct drm_device *dev,
				    struct drm_file *file,
				    struct intel_ring_buffer *ring)
{
	struct drm_i915_gem_request *request;
	u32 invalidate;

	 
	invalidate = I915_GEM_DOMAIN_COMMAND;
	if (INTEL_INFO(dev)->gen >= 4)
		invalidate |= I915_GEM_DOMAIN_SAMPLER;
	if (ring->flush(ring, invalidate, 0)) {
		i915_gem_next_request_seqno(ring);
		return;
	}

	 
	request = kzalloc(sizeof(*request), GFP_KERNEL);
	if (request == NULL || i915_add_request(ring, file, request)) {
		i915_gem_next_request_seqno(ring);
		kfree(request);
	}
}
