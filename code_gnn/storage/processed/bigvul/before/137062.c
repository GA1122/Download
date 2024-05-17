Decimal InputType::FindStepBase(const Decimal& default_value) const {
  Decimal step_base =
      ParseToNumber(GetElement().FastGetAttribute(minAttr), Decimal::Nan());
  if (!step_base.IsFinite())
    step_base =
        ParseToNumber(GetElement().FastGetAttribute(valueAttr), default_value);
  return step_base;
}
