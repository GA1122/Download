void GuestViewBase::SetAttachParams(const base::DictionaryValue& params) {
  attach_params_.reset(params.DeepCopy());
  attach_params_->GetInteger(guestview::kParameterInstanceId,
                             &view_instance_id_);
}
