void AutocompleteEditModel::SetInputInProgress(bool in_progress) {
  if (user_input_in_progress_ == in_progress)
    return;

  user_input_in_progress_ = in_progress;
  if (user_input_in_progress_)
    time_user_first_modified_omnibox_ = base::TimeTicks::Now();
  controller_->OnInputInProgress(in_progress);
}
