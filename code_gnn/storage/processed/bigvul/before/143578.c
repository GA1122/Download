void OomInterventionTabHelper::ResetInterfaces() {
  intervention_.reset();
  if (binding_.is_bound())
    binding_.Close();
}
