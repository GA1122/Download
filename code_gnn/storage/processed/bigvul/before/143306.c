Document::~Document()
{
    DCHECK(!layoutView());
    DCHECK(!parentTreeScope());
    DCHECK(!m_axObjectCache);
    InstanceCounters::decrementCounter(InstanceCounters::DocumentCounter);
}
