void HTMLFormElement::SetDemoted(bool demoted) {
  if (demoted)
    UseCounter::Count(GetDocument(), WebFeature::kDemotedFormElement);
  was_demoted_ = demoted;
}
