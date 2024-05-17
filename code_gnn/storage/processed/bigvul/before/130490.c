bool CommandLatencyQuery::End(base::subtle::Atomic32 submit_count) {
    base::TimeDelta now = base::TimeTicks::HighResNow() - base::TimeTicks();
    MarkAsPending(submit_count);
    return MarkAsCompleted(now.InMicroseconds());
}
