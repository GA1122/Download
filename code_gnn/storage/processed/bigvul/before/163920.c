ScriptPromise ImageCapture::takePhoto(ScriptState* script_state,
                                      const PhotoSettings& photo_settings) {
  return setOptions(script_state, photo_settings,
                    true  );
}
