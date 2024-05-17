void LayerTreeHost::SetAnimationEvents(
    scoped_ptr<AnimationEventsVector> events) {
  DCHECK(proxy_->IsMainThread());
  for (size_t event_index = 0; event_index < events->size(); ++event_index) {
    int event_layer_id = (*events)[event_index].layer_id;

    const AnimationRegistrar::AnimationControllerMap& animation_controllers =
        animation_registrar_->all_animation_controllers();
    AnimationRegistrar::AnimationControllerMap::const_iterator iter =
        animation_controllers.find(event_layer_id);
    if (iter != animation_controllers.end()) {
      switch ((*events)[event_index].type) {
        case AnimationEvent::Started:
          (*iter).second->NotifyAnimationStarted((*events)[event_index]);
          break;

        case AnimationEvent::Finished:
          (*iter).second->NotifyAnimationFinished((*events)[event_index]);
          break;

        case AnimationEvent::Aborted:
          (*iter).second->NotifyAnimationAborted((*events)[event_index]);
          break;

        case AnimationEvent::PropertyUpdate:
          (*iter).second->NotifyAnimationPropertyUpdate((*events)[event_index]);
          break;
      }
    }
  }
}
