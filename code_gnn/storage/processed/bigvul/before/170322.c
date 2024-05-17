int SampleTable::CompareIncreasingTime(const void *_a, const void *_b) {
 const SampleTimeEntry *a = (const SampleTimeEntry *)_a;
 const SampleTimeEntry *b = (const SampleTimeEntry *)_b;

 if (a->mCompositionTime < b->mCompositionTime) {
 return -1;
 } else if (a->mCompositionTime > b->mCompositionTime) {
 return 1;
 }

 return 0;
}
