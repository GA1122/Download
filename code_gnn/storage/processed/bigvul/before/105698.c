void CrosMock::SetInputMethodLibraryStatusAreaExpectations() {
  EXPECT_CALL(*mock_input_method_library_, AddObserver(_))
      .Times(AnyNumber())
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_input_method_library_, GetActiveInputMethods())
      .Times(AnyNumber())
      .WillRepeatedly(InvokeWithoutArgs(CreateInputMethodDescriptors))
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_input_method_library_, GetSupportedInputMethods())
      .Times(AnyNumber())
      .WillRepeatedly(InvokeWithoutArgs(CreateInputMethodDescriptors))
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_input_method_library_, GetKeyboardOverlayId(_))
      .Times(AnyNumber())
      .WillRepeatedly((Return("en_US")))
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_input_method_library_, current_input_method())
      .Times(AnyNumber())
      .WillRepeatedly((Return(current_input_method_)))
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_input_method_library_, previous_input_method())
      .Times(AnyNumber())
      .WillRepeatedly((Return(previous_input_method_)))
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_input_method_library_, current_ime_properties())
      .Times(AnyNumber())
      .WillRepeatedly((ReturnRef(ime_properties_)))
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_input_method_library_, GetNumActiveInputMethods())
      .Times(AnyNumber())
      .WillRepeatedly((Return(1)))
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_input_method_library_, SetImeConfig(_, _, _))
      .Times(AnyNumber())
      .WillRepeatedly((Return(true)))
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_input_method_library_, RemoveObserver(_))
      .Times(AnyNumber())
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_input_method_library_, SetDeferImeStartup(_))
      .Times(AnyNumber())
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_input_method_library_, StopInputMethodDaemon())
      .Times(AnyNumber())
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_input_method_library_, ChangeInputMethod(_))
      .Times(AnyNumber())
      .RetiresOnSaturation();
}
