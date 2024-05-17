    virtual size_t GetNumActiveInputMethods() {
    scoped_ptr<InputMethodDescriptors> descriptors(GetActiveInputMethods());
//     scoped_ptr<input_method::InputMethodDescriptors> descriptors(
//         GetActiveInputMethods());
      return descriptors->size();
    }