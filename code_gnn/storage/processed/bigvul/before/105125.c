PassRefPtr<ClientRect> Range::getBoundingClientRect() const
{
    return ClientRect::create(boundingRect());
}
