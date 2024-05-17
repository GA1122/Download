void PasswordAutofillAgent::OnAutofillUsernameAndPasswordDataReceived(
    const FormsPredictionsMap& predictions) {
  form_predictions_.insert(predictions.begin(), predictions.end());
}
