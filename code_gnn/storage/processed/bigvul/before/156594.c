std::unique_ptr<syncer::EntityData> MoveToEntityData(
    const std::string& client_name,
    SessionSpecifics* specifics) {
  auto entity_data = std::make_unique<syncer::EntityData>();
  entity_data->non_unique_name = client_name;
  if (specifics->has_header()) {
    entity_data->non_unique_name += " (header)";
  } else if (specifics->has_tab()) {
    entity_data->non_unique_name +=
        base::StringPrintf(" (tab node %d)", specifics->tab_node_id());
  }
  entity_data->specifics.mutable_session()->Swap(specifics);
  return entity_data;
}
