bool AutofillManager::GetHost(const std::vector<AutofillProfile*>& profiles,
                              const std::vector<CreditCard*>& credit_cards,
                              RenderViewHost** host) const {
  if (!IsAutofillEnabled())
    return false;

  if (profiles.empty() && credit_cards.empty())
    return false;

  *host = tab_contents()->render_view_host();
  if (!*host)
    return false;

  return true;
}
