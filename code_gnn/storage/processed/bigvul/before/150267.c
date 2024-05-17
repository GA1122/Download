bool InputEngine::IsImeSupportedByRulebased(const std::string& ime_spec) {
   return rulebased::Engine::IsImeSupported(GetIdFromImeSpec(ime_spec));
 }
