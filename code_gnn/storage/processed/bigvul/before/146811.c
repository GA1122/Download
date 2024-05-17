void Document::RecordDeferredLoadReason(WouldLoadReason reason) {
  DCHECK(would_load_reason_ == WouldLoadReason::kInvalid ||
         reason != WouldLoadReason::kCreated);
  DCHECK(reason != WouldLoadReason::kInvalid);
  DCHECK(GetFrame());
  DCHECK(GetFrame()->IsCrossOriginSubframe());
  if (reason <= would_load_reason_ ||
      !GetFrame()->Loader().StateMachine()->CommittedFirstRealDocumentLoad())
    return;
  for (int i = static_cast<int>(would_load_reason_) + 1;
       i <= static_cast<int>(reason); ++i)
    RecordLoadReasonToHistogram(static_cast<WouldLoadReason>(i));
  would_load_reason_ = reason;
}
