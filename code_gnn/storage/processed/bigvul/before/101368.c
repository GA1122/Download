const sync_pb::AutofillSpecifics& BaseNode::GetAutofillSpecifics() const {
  DCHECK_EQ(syncable::AUTOFILL, GetModelType());
  return GetEntitySpecifics().GetExtension(sync_pb::autofill);
}
