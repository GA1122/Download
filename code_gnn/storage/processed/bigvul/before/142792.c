TextTrackContainer& HTMLMediaElement::EnsureTextTrackContainer() {
  ShadowRoot& shadow_root = EnsureUserAgentShadowRoot();
  AssertShadowRootChildren(shadow_root);

  Node* first_child = shadow_root.firstChild();
  if (first_child && first_child->IsTextTrackContainer())
    return ToTextTrackContainer(*first_child);
  Node* to_be_inserted = first_child;

  if (first_child && (first_child->IsMediaRemotingInterstitial() ||
                      first_child->IsPictureInPictureInterstitial())) {
    Node* second_child = first_child->nextSibling();
    if (second_child && second_child->IsTextTrackContainer())
      return ToTextTrackContainer(*second_child);
    to_be_inserted = second_child;
  }

  TextTrackContainer* text_track_container = TextTrackContainer::Create(*this);

  shadow_root.InsertBefore(text_track_container, to_be_inserted);

  AssertShadowRootChildren(shadow_root);

  return *text_track_container;
}
