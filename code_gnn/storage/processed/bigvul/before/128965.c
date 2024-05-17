    bool read(v8::Handle<v8::Value>* value, CompositeCreator& creator)
    {
        SerializationTag tag;
        if (!readTag(&tag))
            return false;
        switch (tag) {
        case ReferenceCountTag: {
            if (!m_version)
                return false;
            uint32_t referenceTableSize;
            if (!doReadUint32(&referenceTableSize))
                return false;
            if (referenceTableSize != creator.objectReferenceCount())
                return false;
            return true;
        }
        case InvalidTag:
            return false;
        case PaddingTag:
            return true;
        case UndefinedTag:
            *value = v8::Undefined(isolate());
            break;
        case NullTag:
            *value = v8::Null(isolate());
            break;
        case TrueTag:
            *value = v8Boolean(true, isolate());
            break;
        case FalseTag:
            *value = v8Boolean(false, isolate());
            break;
        case TrueObjectTag:
            *value = v8::BooleanObject::New(true);
            creator.pushObjectReference(*value);
            break;
        case FalseObjectTag:
            *value = v8::BooleanObject::New(false);
            creator.pushObjectReference(*value);
            break;
        case StringTag:
            if (!readString(value))
                return false;
            break;
        case StringUCharTag:
            if (!readUCharString(value))
                return false;
            break;
        case StringObjectTag:
            if (!readStringObject(value))
                return false;
            creator.pushObjectReference(*value);
            break;
        case Int32Tag:
            if (!readInt32(value))
                return false;
            break;
        case Uint32Tag:
            if (!readUint32(value))
                return false;
            break;
        case DateTag:
            if (!readDate(value))
                return false;
            creator.pushObjectReference(*value);
            break;
        case NumberTag:
            if (!readNumber(value))
                return false;
            break;
        case NumberObjectTag:
            if (!readNumberObject(value))
                return false;
            creator.pushObjectReference(*value);
            break;
        case BlobTag:
        case BlobIndexTag:
            if (!readBlob(value, tag == BlobIndexTag))
                return false;
            creator.pushObjectReference(*value);
            break;
        case FileTag:
        case FileIndexTag:
            if (!readFile(value, tag == FileIndexTag))
                return false;
            creator.pushObjectReference(*value);
            break;
        case DOMFileSystemTag:
            if (!readDOMFileSystem(value))
                return false;
            creator.pushObjectReference(*value);
            break;
        case FileListTag:
        case FileListIndexTag:
            if (!readFileList(value, tag == FileListIndexTag))
                return false;
            creator.pushObjectReference(*value);
            break;
        case CryptoKeyTag:
            if (!readCryptoKey(value))
                return false;
            creator.pushObjectReference(*value);
            break;
        case ImageDataTag:
            if (!readImageData(value))
                return false;
            creator.pushObjectReference(*value);
            break;

        case RegExpTag:
            if (!readRegExp(value))
                return false;
            creator.pushObjectReference(*value);
            break;
        case ObjectTag: {
            uint32_t numProperties;
            if (!doReadUint32(&numProperties))
                return false;
            if (!creator.completeObject(numProperties, value))
                return false;
            break;
        }
        case SparseArrayTag: {
            uint32_t numProperties;
            uint32_t length;
            if (!doReadUint32(&numProperties))
                return false;
            if (!doReadUint32(&length))
                return false;
            if (!creator.completeSparseArray(numProperties, length, value))
                return false;
            break;
        }
        case DenseArrayTag: {
            uint32_t numProperties;
            uint32_t length;
            if (!doReadUint32(&numProperties))
                return false;
            if (!doReadUint32(&length))
                return false;
            if (!creator.completeDenseArray(numProperties, length, value))
                return false;
            break;
        }
        case ArrayBufferViewTag: {
            if (!m_version)
                return false;
            if (!readArrayBufferView(value, creator))
                return false;
            creator.pushObjectReference(*value);
            break;
        }
        case ArrayBufferTag: {
            if (!m_version)
                return false;
            if (!readArrayBuffer(value))
                return false;
            creator.pushObjectReference(*value);
            break;
        }
        case GenerateFreshObjectTag: {
            if (!m_version)
                return false;
            if (!creator.newObject())
                return false;
            return true;
        }
        case GenerateFreshSparseArrayTag: {
            if (!m_version)
                return false;
            uint32_t length;
            if (!doReadUint32(&length))
                return false;
            if (!creator.newSparseArray(length))
                return false;
            return true;
        }
        case GenerateFreshDenseArrayTag: {
            if (!m_version)
                return false;
            uint32_t length;
            if (!doReadUint32(&length))
                return false;
            if (!creator.newDenseArray(length))
                return false;
            return true;
        }
        case MessagePortTag: {
            if (!m_version)
                return false;
            uint32_t index;
            if (!doReadUint32(&index))
                return false;
            if (!creator.tryGetTransferredMessagePort(index, value))
                return false;
            break;
        }
        case ArrayBufferTransferTag: {
            if (!m_version)
                return false;
            uint32_t index;
            if (!doReadUint32(&index))
                return false;
            if (!creator.tryGetTransferredArrayBuffer(index, value))
                return false;
            break;
        }
        case ObjectReferenceTag: {
            if (!m_version)
                return false;
            uint32_t reference;
            if (!doReadUint32(&reference))
                return false;
            if (!creator.tryGetObjectFromObjectReference(reference, value))
                return false;
            break;
        }
        default:
            return false;
        }
        return !value->IsEmpty();
    }
