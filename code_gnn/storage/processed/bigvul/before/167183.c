void HTMLMediaElement::Trace(blink::Visitor* visitor) {
  visitor->Trace(played_time_ranges_);
  visitor->Trace(async_event_queue_);
  visitor->Trace(error_);
  visitor->Trace(current_source_node_);
  visitor->Trace(next_child_node_to_consider_);
  visitor->Trace(media_source_);
  visitor->Trace(audio_tracks_);
  visitor->Trace(video_tracks_);
  visitor->Trace(cue_timeline_);
  visitor->Trace(text_tracks_);
  visitor->Trace(text_tracks_when_resource_selection_began_);
  visitor->Trace(play_promise_resolvers_);
  visitor->Trace(play_promise_resolve_list_);
  visitor->Trace(play_promise_reject_list_);
  visitor->Trace(audio_source_provider_);
  visitor->Trace(src_object_);
  visitor->Trace(autoplay_policy_);
  visitor->Trace(media_controls_);
  visitor->Trace(controls_list_);
  visitor->template RegisterWeakMembers<HTMLMediaElement,
                                        &HTMLMediaElement::ClearWeakMembers>(
      this);
  Supplementable<HTMLMediaElement>::Trace(visitor);
  HTMLElement::Trace(visitor);
  PausableObject::Trace(visitor);
}