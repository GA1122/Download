DEFINE_TRACE(RuleFeatureSet)
{
#if ENABLE(OILPAN)
    visitor->trace(siblingRules);
    visitor->trace(uncommonAttributeRules);
#endif
}
