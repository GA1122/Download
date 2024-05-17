bool ArcVoiceInteractionFrameworkService::IsHomescreenActive() {
  if (chromeos::GetAshConfig() == ash::Config::MASH)
    return false;
  return !ash::Shell::Get()->activation_client()->GetActiveWindow();
}
