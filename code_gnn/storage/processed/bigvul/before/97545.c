  void TestSwitchDisablesType(const char* cmd_switch,
                              syncer::ModelType type) {
    command_line_->AppendSwitch(cmd_switch);
    scoped_ptr<ProfileSyncService> pss(
        new ProfileSyncService(
            new ProfileSyncComponentsFactoryImpl(profile_.get(),
                                                 command_line_.get()),
            profile_.get(),
            NULL,
            ProfileSyncService::MANUAL_START));
    pss->factory()->RegisterDataTypes(pss.get());
    DataTypeController::StateMap controller_states;
    pss->GetDataTypeControllerStates(&controller_states);
    EXPECT_EQ(DefaultDatatypesCount() - 1, controller_states.size());
    CheckDefaultDatatypesInMapExcept(&controller_states, type);
  }
