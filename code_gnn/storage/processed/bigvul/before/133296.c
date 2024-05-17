void HandleSwapPrimaryDisplay() {
  base::RecordAction(UserMetricsAction("Accel_Swap_Primary_Display"));
  Shell::GetInstance()->display_configuration_controller()->SetPrimaryDisplayId(
      ScreenUtil::GetSecondaryDisplay().id(), true  );
}
