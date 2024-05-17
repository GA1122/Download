    void writeArrayBufferView(const ArrayBufferView& arrayBufferView)
    {
        append(ArrayBufferViewTag);
#if ENABLE(ASSERT)
        const ArrayBuffer& arrayBuffer = *arrayBufferView.buffer();
        ASSERT(static_cast<const uint8_t*>(arrayBuffer.data()) + arrayBufferView.byteOffset() ==
               static_cast<const uint8_t*>(arrayBufferView.baseAddress()));
#endif
        ArrayBufferView::ViewType type = arrayBufferView.type();

        if (type == ArrayBufferView::TypeInt8)
            append(ByteArrayTag);
        else if (type == ArrayBufferView::TypeUint8Clamped)
            append(UnsignedByteClampedArrayTag);
        else if (type == ArrayBufferView::TypeUint8)
            append(UnsignedByteArrayTag);
        else if (type == ArrayBufferView::TypeInt16)
            append(ShortArrayTag);
        else if (type == ArrayBufferView::TypeUint16)
            append(UnsignedShortArrayTag);
        else if (type == ArrayBufferView::TypeInt32)
            append(IntArrayTag);
        else if (type == ArrayBufferView::TypeUint32)
            append(UnsignedIntArrayTag);
        else if (type == ArrayBufferView::TypeFloat32)
            append(FloatArrayTag);
        else if (type == ArrayBufferView::TypeFloat64)
            append(DoubleArrayTag);
        else if (type == ArrayBufferView::TypeDataView)
            append(DataViewTag);
        else
            ASSERT_NOT_REACHED();
        doWriteUint32(arrayBufferView.byteOffset());
        doWriteUint32(arrayBufferView.byteLength());
    }
