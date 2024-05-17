bool UseBackgroundMode() {
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kDisablePushApiBackgroundMode))
    return false;
  if (command_line->HasSwitch(switches::kEnablePushApiBackgroundMode))
    return true;
  return base::FeatureList::IsEnabled(features::kPushMessagingBackgroundMode);
}
