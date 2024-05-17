syncable::ModelTypeSet GetTypesWithEmptyProgressMarkerToken(
    syncable::ModelTypeSet types,
    sync_api::UserShare* share) {
  syncable::ModelTypeSet result;
  for (syncable::ModelTypeSet::Iterator i = types.First();
       i.Good(); i.Inc()) {
    sync_pb::DataTypeProgressMarker marker;
    share->directory->GetDownloadProgress(i.Get(), &marker);

    if (marker.token().empty())
      result.Put(i.Get());

  }
  return result;
}
