void Part::slotAddFiles()
{
    if (!m_compressionOptions.isCompressionLevelSet() && arguments().metaData().contains(QStringLiteral("compressionLevel"))) {
        m_compressionOptions.setCompressionLevel(arguments().metaData()[QStringLiteral("compressionLevel")].toInt());
    }
    if (m_compressionOptions.compressionMethod().isEmpty() && arguments().metaData().contains(QStringLiteral("compressionMethod"))) {
        m_compressionOptions.setCompressionMethod(arguments().metaData()[QStringLiteral("compressionMethod")]);
    }
    if (m_compressionOptions.encryptionMethod().isEmpty() && arguments().metaData().contains(QStringLiteral("encryptionMethod"))) {
        m_compressionOptions.setEncryptionMethod(arguments().metaData()[QStringLiteral("encryptionMethod")]);
    }
    if (!m_compressionOptions.isVolumeSizeSet() && arguments().metaData().contains(QStringLiteral("volumeSize"))) {
        m_compressionOptions.setVolumeSize(arguments().metaData()[QStringLiteral("volumeSize")].toInt());
    }

    const auto compressionMethods = m_model->archive()->property("compressionMethods").toStringList();
    qCDebug(ARK) << "compmethods:" << compressionMethods;
    if (compressionMethods.size() == 1) {
        m_compressionOptions.setCompressionMethod(compressionMethods.first());
    }

    QString dialogTitle = i18nc("@title:window", "Add Files");
    const Archive::Entry *destination = Q_NULLPTR;
    if (m_view->selectionModel()->selectedRows().count() == 1) {
        destination = m_model->entryForIndex(m_view->selectionModel()->currentIndex());
        if (destination->isDir()) {
            dialogTitle = i18nc("@title:window", "Add Files to %1", destination->fullPath());;
        } else {
            destination = Q_NULLPTR;
        }
    }

    qCDebug(ARK) << "Opening AddDialog with opts:" << m_compressionOptions;


    QPointer<AddDialog> dlg = new AddDialog(widget(),
                                            dialogTitle,
                                            m_lastUsedAddPath,
                                            m_model->archive()->mimeType(),
                                            m_compressionOptions);

    if (dlg->exec() == QDialog::Accepted) {
        qCDebug(ARK) << "Selected files:" << dlg->selectedFiles();
        qCDebug(ARK) << "Options:" << dlg->compressionOptions();
        m_compressionOptions = dlg->compressionOptions();
        slotAddFiles(dlg->selectedFiles(), destination, QString());
    }
    delete dlg;
}
