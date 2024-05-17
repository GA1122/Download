static const InputTypeFactoryMap* FactoryMap() {
  static const InputTypeFactoryMap* factory_map =
      CreateInputTypeFactoryMap().release();
  return factory_map;
}
