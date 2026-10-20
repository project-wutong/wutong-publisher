// SPDX-License-Identifier: AGPL-3.0-only
#pragma once
#include <QMainWindow>

class QLabel;
class QPlainTextEdit;

// GUI layer only: all document/protocol logic lives in the Rust engine behind the cxx bridge.
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void describe(const QString &path);

private:
    QLabel *m_status = nullptr;
    QPlainTextEdit *m_log = nullptr;
};
