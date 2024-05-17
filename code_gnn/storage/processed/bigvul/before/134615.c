bool OSExchangeDataProviderAura::GetString(base::string16* data) const {
  if ((formats_ & OSExchangeData::STRING) == 0)
    return false;
  *data = string_;
   return true;
 }
