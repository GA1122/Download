void CrosMock::SetNetworkLibraryStatusAreaExpectations() {
  EXPECT_CALL(*mock_network_library_, AddNetworkManagerObserver(_))
      .Times(AnyNumber());
  EXPECT_CALL(*mock_network_library_, AddNetworkDeviceObserver(_, _))
      .Times(AnyNumber());
  EXPECT_CALL(*mock_network_library_, AddCellularDataPlanObserver(_))
      .Times(AnyNumber());
  EXPECT_CALL(*mock_network_library_, RemoveNetworkManagerObserver(_))
      .Times(AnyNumber());
  EXPECT_CALL(*mock_network_library_, RemoveNetworkDeviceObserver(_, _))
      .Times(AnyNumber());
  EXPECT_CALL(*mock_network_library_, RemoveObserverForAllNetworks(_))
      .Times(AnyNumber());
  EXPECT_CALL(*mock_network_library_, RemoveCellularDataPlanObserver(_))
      .Times(AnyNumber());
  EXPECT_CALL(*mock_network_library_, IsLocked())
      .Times(AnyNumber())
      .WillRepeatedly((Return(false)));
  EXPECT_CALL(*mock_network_library_, FindCellularDevice())
      .Times(AnyNumber())
      .WillRepeatedly((Return((const NetworkDevice*)(NULL))));
  EXPECT_CALL(*mock_network_library_, ethernet_available())
      .Times(AnyNumber())
      .WillRepeatedly((Return(true)));
  EXPECT_CALL(*mock_network_library_, wifi_available())
      .Times(AnyNumber())
      .WillRepeatedly((Return(false)));
  EXPECT_CALL(*mock_network_library_, cellular_available())
      .Times(AnyNumber())
      .WillRepeatedly((Return(false)));
  EXPECT_CALL(*mock_network_library_, ethernet_enabled())
      .Times(AnyNumber())
      .WillRepeatedly((Return(true)));
  EXPECT_CALL(*mock_network_library_, wifi_enabled())
      .Times(AnyNumber())
      .WillRepeatedly((Return(false)));
  EXPECT_CALL(*mock_network_library_, cellular_enabled())
      .Times(AnyNumber())
      .WillRepeatedly((Return(false)));
  EXPECT_CALL(*mock_network_library_, active_network())
      .Times(AnyNumber())
      .WillRepeatedly((Return((const Network*)(NULL))));
  EXPECT_CALL(*mock_network_library_, wifi_network())
      .Times(AnyNumber())
      .WillRepeatedly((Return((const WifiNetwork*)(NULL))));
  EXPECT_CALL(*mock_network_library_, cellular_network())
      .Times(AnyNumber())
      .WillRepeatedly((Return((const CellularNetwork*)(NULL))));
  EXPECT_CALL(*mock_network_library_, virtual_network())
      .Times(AnyNumber())
      .WillRepeatedly((Return((const VirtualNetwork*)(NULL))));
  EXPECT_CALL(*mock_network_library_, wifi_networks())
      .Times(AnyNumber())
      .WillRepeatedly((ReturnRef(wifi_networks_)));
  EXPECT_CALL(*mock_network_library_, cellular_networks())
      .Times(AnyNumber())
      .WillRepeatedly((ReturnRef(cellular_networks_)));
  EXPECT_CALL(*mock_network_library_, virtual_networks())
      .Times(AnyNumber())
      .WillRepeatedly((ReturnRef(virtual_networks_)));
  EXPECT_CALL(*mock_network_library_, connected_network())
      .Times(AnyNumber())
      .WillRepeatedly((Return((const Network*)(NULL))));


  EXPECT_CALL(*mock_network_library_, Connected())
      .Times(AnyNumber())
      .WillRepeatedly((Return(false)))
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_network_library_, Connecting())
      .Times(AnyNumber())
      .WillRepeatedly((Return(false)))
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_network_library_, cellular_connected())
      .Times(AnyNumber())
      .WillRepeatedly((Return(false)))
      .RetiresOnSaturation();

  EXPECT_CALL(*mock_network_library_, IsLocked())
      .Times(AnyNumber())
      .WillRepeatedly((Return(false)))
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_network_library_, ethernet_connected())
      .Times(1)
      .WillRepeatedly((Return(false)))
      .RetiresOnSaturation();
  EXPECT_CALL(*mock_network_library_, ethernet_connecting())
      .Times(1)
      .WillRepeatedly((Return(false)))
      .RetiresOnSaturation();
}
