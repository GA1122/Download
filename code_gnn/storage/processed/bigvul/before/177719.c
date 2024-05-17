int equalizer_set_device(effect_context_t *context, uint32_t device)
{
    ALOGV("%s: device: %d", __func__, device);
 equalizer_context_t *eq_ctxt = (equalizer_context_t *)context;
    eq_ctxt->device = device;
    offload_eq_set_device(&(eq_ctxt->offload_eq), device);
 return 0;
}
