bool ReadValue(const Message* m, PickleIterator* iter, Value** value,
               int recursion) {
  if (recursion > kMaxRecursionDepth) {
    LOG(WARNING) << "Max recursion depth hit in ReadValue.";
    return false;
  }

  int type;
  if (!ReadParam(m, iter, &type))
    return false;

  switch (type) {
    case Value::TYPE_NULL:
    *value = Value::CreateNullValue();
    break;
    case Value::TYPE_BOOLEAN: {
      bool val;
      if (!ReadParam(m, iter, &val))
        return false;
      *value = Value::CreateBooleanValue(val);
      break;
    }
    case Value::TYPE_INTEGER: {
      int val;
      if (!ReadParam(m, iter, &val))
        return false;
      *value = Value::CreateIntegerValue(val);
      break;
    }
    case Value::TYPE_DOUBLE: {
      double val;
      if (!ReadParam(m, iter, &val))
        return false;
      *value = Value::CreateDoubleValue(val);
      break;
    }
    case Value::TYPE_STRING: {
      std::string val;
      if (!ReadParam(m, iter, &val))
        return false;
      *value = Value::CreateStringValue(val);
      break;
    }
    case Value::TYPE_BINARY: {
      const char* data;
      int length;
      if (!m->ReadData(iter, &data, &length))
        return false;
      *value = base::BinaryValue::CreateWithCopiedBuffer(data, length);
      break;
    }
    case Value::TYPE_DICTIONARY: {
      scoped_ptr<DictionaryValue> val(new DictionaryValue());
      if (!ReadDictionaryValue(m, iter, val.get(), recursion))
        return false;
      *value = val.release();
      break;
    }
    case Value::TYPE_LIST: {
      scoped_ptr<ListValue> val(new ListValue());
      if (!ReadListValue(m, iter, val.get(), recursion))
        return false;
      *value = val.release();
      break;
    }
    default:
    return false;
  }

  return true;
}
