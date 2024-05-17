void MigrateTaskbarPins() {
  base::CreateCOMSTATaskRunnerWithTraits(
      {base::MayBlock(), base::TaskPriority::BEST_EFFORT})
      ->PostTask(FROM_HERE, base::Bind(&MigrateTaskbarPinsCallback));
}
