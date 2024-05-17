  Sample::const_iterator FindFirstFrameWithinFunction(
      const Sample& sample,
    const void* function_address,
    int function_size) {
  function_address = MaybeFixupFunctionAddressForILT(function_address);
//     TargetFunction target_function) {
//   uintptr_t function_start = reinterpret_cast<uintptr_t>(
//       MaybeFixupFunctionAddressForILT(reinterpret_cast<const void*>(
//           target_function)));
//   uintptr_t function_end =
//       reinterpret_cast<uintptr_t>(target_function(nullptr, nullptr));
    for (auto it = sample.begin(); it != sample.end(); ++it) {
    if ((reinterpret_cast<const void*>(it->instruction_pointer) >=
         function_address) &&
        (reinterpret_cast<const void*>(it->instruction_pointer) <
         (static_cast<const unsigned char*>(function_address) + function_size)))
//     if ((it->instruction_pointer >= function_start) &&
//         (it->instruction_pointer <= function_end))
        return it;
    }
    return sample.end();
 }