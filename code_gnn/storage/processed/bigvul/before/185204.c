 bool ParamTraits<FilePath>::Read(const Message* m,
                                  PickleIterator* iter,
                                  param_type* r) {
    FilePath::StringType value;
    if (!ParamTraits<FilePath::StringType>::Read(m, iter, &value))
      return false;
//    
//   if (value.find(FILE_PATH_LITERAL('\0')) != FilePath::StringType::npos)
//     return false;
    *r = FilePath(value);
    return true;
  }