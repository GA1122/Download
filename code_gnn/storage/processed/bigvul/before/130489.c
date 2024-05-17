bool CommandsIssuedQuery::End(base::subtle::Atomic32 submit_count) {
  base::TimeDelta elapsed = base::TimeTicks::HighResNow() - begin_time_;
  MarkAsPending(submit_count);
  return MarkAsCompleted(elapsed.InMicroseconds());
}
