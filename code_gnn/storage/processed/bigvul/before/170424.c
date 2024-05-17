bool Parcel::checkInterface(IBinder* binder) const
{
 return enforceInterface(binder->getInterfaceDescriptor());
}
