    OptimizationHintsComponentInstallerPolicy()
    : ruleset_format_version_(
          base::Version(optimization_guide::kRulesetFormatVersionString)) {
  DCHECK(ruleset_format_version_.IsValid());
}
