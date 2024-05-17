PassRefPtrWillBeRawPtr<HTMLAllCollection> Document::allForBinding()
{
    UseCounter::count(*this, UseCounter::DocumentAll);
    return all();
}
