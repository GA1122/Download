bool CommandsIssuedQuery::Begin() {
  begin_time_ = base::TimeTicks::HighResNow();
  return true;
}
