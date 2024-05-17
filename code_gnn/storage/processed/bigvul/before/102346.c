bool PolicyStatusInfo::Equals(const PolicyStatusInfo* other_info) const {
  return name == other_info->name &&
         source_type == other_info->source_type &&
         level == other_info->level &&
         value->Equals(other_info->value.get()) &&
         status == other_info->status &&
         error_message == other_info->error_message;
}
