bool IsVoiceInteractionFlagsEnabled() {
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  return !IsAssistantFlagsEnabled() &&
         (command_line->HasSwitch(kEnableVoiceInteraction) ||
          base::FeatureList::IsEnabled(kVoiceInteractionFeature));
}
