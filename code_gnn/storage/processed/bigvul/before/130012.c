VariationsRequestSchedulerMobile::VariationsRequestSchedulerMobile(
    const base::Closure& task,
    PrefService* local_state) :
  VariationsRequestScheduler(task), local_state_(local_state) {
}
