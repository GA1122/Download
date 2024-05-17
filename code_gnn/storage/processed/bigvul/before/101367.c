const AutofillProfileSpecifics& BaseNode::GetAutofillProfileSpecifics() const {
  DCHECK_EQ(GetModelType(), syncable::AUTOFILL_PROFILE);
  return GetEntitySpecifics().GetExtension(sync_pb::autofill_profile);
}
