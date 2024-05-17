  virtual void AddCellularDataPlanObserver(CellularDataPlanObserver* observer) {
    if (!data_plan_observers_.HasObserver(observer))
      data_plan_observers_.AddObserver(observer);
  }
