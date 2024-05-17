base::StringPiece TestContentClient::GetDataResource(int resource_id) const {
  base::StringPiece resource;
  data_pack_.GetStringPiece(resource_id, &resource);
  return resource;
}
