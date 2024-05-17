static int fx_get_descriptor(effect_handle_t   self,
 effect_descriptor_t *pDescriptor)
{
 struct effect_s *effect = (struct effect_s *)self;

 if (effect == NULL || pDescriptor == NULL)
 return -EINVAL;

 *pDescriptor = *descriptors[effect->id];

 return 0;
}
