void JSFloat64Array::finishCreation(JSGlobalData& globalData)
{
    Base::finishCreation(globalData);
    TypedArrayDescriptor descriptor(&JSFloat64Array::s_info, OBJECT_OFFSETOF(JSFloat64Array, m_storage), OBJECT_OFFSETOF(JSFloat64Array, m_storageLength));
    globalData.registerTypedArrayDescriptor(impl(), descriptor);
    m_storage = impl()->data();
    m_storageLength = impl()->length();
    ASSERT(inherits(&s_info));
}
