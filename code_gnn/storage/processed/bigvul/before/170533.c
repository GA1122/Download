int Downmix_GetDescriptor(effect_handle_t self, effect_descriptor_t *pDescriptor)
{
 downmix_module_t *pDwnmxModule = (downmix_module_t *) self;

 if (pDwnmxModule == NULL ||
            pDwnmxModule->context.state == DOWNMIX_STATE_UNINITIALIZED) {
 return -EINVAL;
 }

    memcpy(pDescriptor, &gDownmixDescriptor, sizeof(effect_descriptor_t));

 return 0;
}
