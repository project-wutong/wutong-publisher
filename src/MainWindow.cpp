// SPDX-License-Identifier: AGPL-3.0-only
#include "MainWindow.h"
#include "rust/cxx.h"
#include "wutong-publisher-bridge/src/lib.rs.h"

#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle(QStringLiteral(WUTONG_APP_TITLE));
    m_log = new QPlainTextEdit(this);
    m_log->setReadOnly(true);
    setCentralWidget(m_log);
    m_status = new QLabel(this);
    statusBar()->addPermanentWidget(m_status);
    const auto ver = wutong::publisher::core_version();
    m_status->setText(QString::fromUtf8(ver.data(), static_cast<qsizetype>(ver.size())));
    auto *file = menuBar()->addMenu(tr("&File"));
    file->addAction(tr("&Open…"), this, [this] {
        const QString p = QFileDialog::getOpenFileName(this, tr("Open"));
        if (!p.isEmpty()) describe(p);
    });
    file->addAction(tr("&Quit"), this, &QWidget::close);
    resize(960, 640);
}

void MainWindow::describe(const QString &path) {
    const QByteArray utf8 = path.toUtf8();
    try {
        const auto s = wutong::publisher::describe_path(rust::Str(utf8.constData(), static_cast<size_t>(utf8.size())));
        m_log->appendPlainText(path + QStringLiteral(": ") + QString::fromUtf8(s.data(), static_cast<qsizetype>(s.size())));
    } catch (const rust::Error &e) {
        m_log->appendPlainText(path + QStringLiteral(": error: ") + QString::fromUtf8(e.what()));
    }
}
