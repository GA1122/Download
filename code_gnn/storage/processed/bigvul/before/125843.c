void WriteValue(Message* m, const Value* value, int recursion) {
  bool result;
  if (recursion > kMaxRecursionDepth) {
    LOG(WARNING) << "Max recursion depth hit in WriteValue.";
    return;
  }

  m->WriteInt(value->GetType());

  switch (value->GetType()) {
    case Value::TYPE_NULL:
    break;
    case Value::TYPE_BOOLEAN: {
      bool val;
      result = value->GetAsBoolean(&val);
      DCHECK(result);
      WriteParam(m, val);
      break;
    }
    case Value::TYPE_INTEGER: {
      int val;
      result = value->GetAsInteger(&val);
      DCHECK(result);
      WriteParam(m, val);
      break;
    }
    case Value::TYPE_DOUBLE: {
      double val;
      result = value->GetAsDouble(&val);
      DCHECK(result);
      WriteParam(m, val);
      break;
    }
    case Value::TYPE_STRING: {
      std::string val;
      result = value->GetAsString(&val);
      DCHECK(result);
      WriteParam(m, val);
      break;
    }
    case Value::TYPE_BINARY: {
      const base::BinaryValue* binary =
          static_cast<const base::BinaryValue*>(value);
      m->WriteData(binary->GetBuffer(), static_cast<int>(binary->GetSize()));
      break;
    }
    case Value::TYPE_DICTIONARY: {
      const DictionaryValue* dict = static_cast<const DictionaryValue*>(value);

      WriteParam(m, static_cast<int>(dict->size()));

      for (DictionaryValue::key_iterator it = dict->begin_keys();
           it != dict->end_keys(); ++it) {
        const Value* subval;
        if (dict->GetWithoutPathExpansion(*it, &subval)) {
          WriteParam(m, *it);
          WriteValue(m, subval, recursion + 1);
        } else {
          NOTREACHED() << "DictionaryValue iterators are filthy liars.";
        }
      }
      break;
    }
    case Value::TYPE_LIST: {
      const ListValue* list = static_cast<const ListValue*>(value);
      WriteParam(m, static_cast<int>(list->GetSize()));
      for (size_t i = 0; i < list->GetSize(); ++i) {
        const Value* subval;
        if (list->Get(i, &subval)) {
          WriteValue(m, subval, recursion + 1);
        } else {
          NOTREACHED() << "ListValue::GetSize is a filthy liar.";
        }
      }
      break;
    }
  }
}
