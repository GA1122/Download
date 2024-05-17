int GetNextProviderId() {
  static base::StaticAtomicSequenceNumber sequence;
   return sequence.GetNext();   
 }
