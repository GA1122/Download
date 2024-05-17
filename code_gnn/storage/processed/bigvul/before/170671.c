int Visualizer_getDescriptor(effect_handle_t   self,
 effect_descriptor_t *pDescriptor)
{
 VisualizerContext * pContext = (VisualizerContext *) self;

 if (pContext == NULL || pDescriptor == NULL) {
        ALOGV("Visualizer_getDescriptor() invalid param");
 return -EINVAL;
 }

 *pDescriptor = gVisualizerDescriptor;

 return 0;
}  
