void FeatureInfo::InitializeBasicState(const base::CommandLine* command_line) {
  if (!command_line)
    return;

  feature_flags_.enable_shader_name_hashing =
      !command_line->HasSwitch(switches::kDisableShaderNameHashing);

  feature_flags_.is_swiftshader_for_webgl =
      (command_line->GetSwitchValueASCII(switches::kUseGL) ==
       gl::kGLImplementationSwiftShaderForWebGLName);

  feature_flags_.is_swiftshader =
      (command_line->GetSwitchValueASCII(switches::kUseGL) ==
       gl::kGLImplementationSwiftShaderName);

  disable_shader_translator_ =
      command_line->HasSwitch(switches::kDisableGLSLTranslator);
}
