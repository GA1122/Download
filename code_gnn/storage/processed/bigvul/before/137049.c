static std::unique_ptr<InputTypeFactoryMap> CreateInputTypeFactoryMap() {
  std::unique_ptr<InputTypeFactoryMap> map =
      WTF::WrapUnique(new InputTypeFactoryMap);
  map->insert(InputTypeNames::button, ButtonInputType::Create);
  map->insert(InputTypeNames::checkbox, CheckboxInputType::Create);
  map->insert(InputTypeNames::color, ColorInputType::Create);
  map->insert(InputTypeNames::date, DateInputType::Create);
  map->insert(InputTypeNames::datetime_local, DateTimeLocalInputType::Create);
  map->insert(InputTypeNames::email, EmailInputType::Create);
  map->insert(InputTypeNames::file, FileInputType::Create);
  map->insert(InputTypeNames::hidden, HiddenInputType::Create);
  map->insert(InputTypeNames::image, ImageInputType::Create);
  map->insert(InputTypeNames::month, MonthInputType::Create);
  map->insert(InputTypeNames::number, NumberInputType::Create);
  map->insert(InputTypeNames::password, PasswordInputType::Create);
  map->insert(InputTypeNames::radio, RadioInputType::Create);
  map->insert(InputTypeNames::range, RangeInputType::Create);
  map->insert(InputTypeNames::reset, ResetInputType::Create);
  map->insert(InputTypeNames::search, SearchInputType::Create);
  map->insert(InputTypeNames::submit, SubmitInputType::Create);
  map->insert(InputTypeNames::tel, TelephoneInputType::Create);
  map->insert(InputTypeNames::time, TimeInputType::Create);
  map->insert(InputTypeNames::url, URLInputType::Create);
  map->insert(InputTypeNames::week, WeekInputType::Create);
  return map;
}
