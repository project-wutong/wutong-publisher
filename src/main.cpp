// SPDX-License-Identifier: AGPL-3.0-only
#include "MainWindow.h"
#include "rust/cxx.h"
#include "wutong-publisher-bridge/src/lib.rs.h"

#include <QApplication>
#include <QStringList>
#include <cstdio>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("wutong-publisher"));
    QApplication::setOrganizationName(QStringLiteral("Project Wutong"));
    // Runs fully offline: no account, network, server or AI is required (OFFLINE-001).
    if (app.arguments().contains(QStringLiteral("--smoke-test"))) {
        const auto v = wutong::publisher::core_version();
        std::printf("%s ok (%.*s)\n", "wutong-publisher", static_cast<int>(v.size()), v.data());
        return 0;
    }
    MainWindow w;
    for (const QString &p : app.arguments().mid(1)) w.describe(p);
    w.show();
    return QApplication::exec();
}
