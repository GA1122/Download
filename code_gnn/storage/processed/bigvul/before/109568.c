PassRefPtr<HTMLCollection> Document::allForBinding()
{
    UseCounter::count(*this, UseCounter::DocumentAll);
    return all();
}
