ChromeBrowserMainPartsChromeos::ChromeBrowserMainPartsChromeos(
    const content::MainFunctionParams& parameters,
    std::unique_ptr<ui::DataPack> data_pack)
    : ChromeBrowserMainPartsLinux(parameters, std::move(data_pack)) {}
