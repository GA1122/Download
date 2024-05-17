void AutofillManager::Reset() {
  ProcessPendingFormForUpload();
  DCHECK(!pending_form_data_);
  AutofillHandler::Reset();
  form_interactions_ukm_logger_.reset(
      new AutofillMetrics::FormInteractionsUkmLogger(
          client_->GetUkmRecorder()));
  address_form_event_logger_.reset(new AutofillMetrics::FormEventLogger(
       false, form_interactions_ukm_logger_.get()));
  credit_card_form_event_logger_.reset(new AutofillMetrics::FormEventLogger(
       true, form_interactions_ukm_logger_.get()));
#if defined(OS_ANDROID) || defined(OS_IOS)
  autofill_assistant_.Reset();
#endif
  has_logged_autofill_enabled_ = false;
  has_logged_address_suggestions_count_ = false;
  did_show_suggestions_ = false;
  user_did_type_ = false;
  user_did_autofill_ = false;
  user_did_edit_autofilled_field_ = false;
  enable_ablation_logging_ = false;
  masked_card_ = CreditCard();
  unmasking_query_id_ = -1;
  unmasking_form_ = FormData();
  unmasking_field_ = FormFieldData();
  forms_loaded_timestamps_.clear();
  initial_interaction_timestamp_ = TimeTicks();
  external_delegate_->Reset();
  filling_contexts_map_.clear();
}
