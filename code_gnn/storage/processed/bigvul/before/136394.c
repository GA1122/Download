std::unique_ptr<JSONObject> PaintArtifactCompositor::LayersAsJSON(
    LayerTreeFlags flags) const {
  ContentLayerClientImpl::LayerAsJSONContext context(flags);
  std::unique_ptr<JSONArray> layers_json = JSONArray::Create();
  for (const auto& client : content_layer_clients_) {
    layers_json->PushObject(client->LayerAsJSON(context));
  }
  std::unique_ptr<JSONObject> json = JSONObject::Create();
  json->SetArray("layers", std::move(layers_json));
  if (context.transforms_json)
    json->SetArray("transforms", std::move(context.transforms_json));
  return json;
}
