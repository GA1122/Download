void ChooserContextBase::RevokeObjectPermission(
    const GURL& requesting_origin,
    const GURL& embedding_origin,
    const base::DictionaryValue& object) {
  DCHECK_EQ(requesting_origin, requesting_origin.GetOrigin());
  DCHECK_EQ(embedding_origin, embedding_origin.GetOrigin());
  DCHECK(IsValidObject(object));

  std::unique_ptr<base::DictionaryValue> setting =
      GetWebsiteSetting(requesting_origin, embedding_origin,  nullptr);
  base::ListValue* object_list;
  if (!setting->GetList(kObjectListKey, &object_list))
    return;
  object_list->Remove(object, nullptr);
  SetWebsiteSetting(requesting_origin, embedding_origin, std::move(setting));
  NotifyPermissionRevoked(requesting_origin, embedding_origin);
}
