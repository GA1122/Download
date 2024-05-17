PropertyTreeManager::PropertyTreeManager(PropertyTreeManagerClient& client,
                                         cc::PropertyTrees& property_trees,
                                         cc::Layer* root_layer,
                                         int sequence_number)
    : client_(client),
      property_trees_(property_trees),
      root_layer_(root_layer),
      sequence_number_(sequence_number) {
  SetupRootTransformNode();
  SetupRootClipNode();
  SetupRootEffectNode();
  SetupRootScrollNode();
}
