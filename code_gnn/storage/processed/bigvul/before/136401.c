void PaintArtifactCompositor::SetTracksRasterInvalidations(bool should_track) {
  for (auto& client : content_layer_clients_)
    client->SetTracksRasterInvalidations(should_track);
}
