 CreateEffectWithCompositorElementId(
     const CompositorElementId& compositor_element_id) {
   EffectPaintPropertyNode::State state;
  state.compositor_element_id = compositor_element_id;
  return EffectPaintPropertyNode::Create(EffectPaintPropertyNode::Root(),
                                         std::move(state));
 }
