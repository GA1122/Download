void ReflectItemStatus(const ash::LauncherItem& item,
                       LauncherButton* button) {
  switch (item.status) {
    case STATUS_CLOSED:
      button->ClearState(LauncherButton::STATE_ACTIVE);
      button->ClearState(LauncherButton::STATE_RUNNING);
      button->ClearState(LauncherButton::STATE_ATTENTION);
      button->ClearState(LauncherButton::STATE_PENDING);
      break;
    case STATUS_RUNNING:
      button->ClearState(LauncherButton::STATE_ACTIVE);
      button->AddState(LauncherButton::STATE_RUNNING);
      button->ClearState(LauncherButton::STATE_ATTENTION);
      button->ClearState(LauncherButton::STATE_PENDING);
      break;
    case STATUS_ACTIVE:
      button->AddState(LauncherButton::STATE_ACTIVE);
      button->ClearState(LauncherButton::STATE_RUNNING);
      button->ClearState(LauncherButton::STATE_ATTENTION);
      button->ClearState(LauncherButton::STATE_PENDING);
      break;
    case STATUS_ATTENTION:
      button->ClearState(LauncherButton::STATE_ACTIVE);
      button->ClearState(LauncherButton::STATE_RUNNING);
      button->AddState(LauncherButton::STATE_ATTENTION);
      button->ClearState(LauncherButton::STATE_PENDING);
      break;
    case STATUS_IS_PENDING:
      button->ClearState(LauncherButton::STATE_ACTIVE);
      button->ClearState(LauncherButton::STATE_RUNNING);
      button->ClearState(LauncherButton::STATE_ATTENTION);
      button->AddState(LauncherButton::STATE_PENDING);
      break;
  }
}
