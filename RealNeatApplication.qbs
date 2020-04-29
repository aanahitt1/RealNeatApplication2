import qbs.FileInfo

QtApplication {
    Depends { name: "Qt.widgets" }
    Depends { name: "Qt.xml" }
    Depends { name: "Qt.gui" }
    Depends { name: "Qt.svg" }

    // The following define makes your compiler emit warnings if you use
    // any Qt feature that has been marked deprecated (the exact warnings
    // depend on your compiler). Please consult the documentation of the
    // deprecated API in order to know how to port your code away from it.
    // You can also make your code fail to compile if it uses deprecated APIs.
    // In order to do so, uncomment the second entry in the list.
    // You can also select to disable deprecated APIs only up to a certain version of Qt.
    cpp.defines: [
        "QT_DEPRECATED_WARNINGS",
        /* "QT_DISABLE_DEPRECATED_BEFORE=0x060000" */ // disables all the APIs deprecated before Qt 6.0.0
    ]

    files: [
        "Config.xml",
        "IAlgorithm.h",
        "RESOURCE.qrc",
        "RNALIB/rnalib.cpp",
        "RNALIB/rnalib.h",
        "graphicswindow.cpp",
        "graphicswindow.h",
        "main.cpp",
        "mainwindow.cpp",
        "mainwindow.h",
        "mainwindow.ui",
        "optionchooser.cpp",
        "optionchooser.h",
        "parsexml.cpp",
        "parsexml.h",
        "pluginmanager.cpp",
        "pluginmanager.h",
        "rnamodelgen.cpp",
        "rnamodelgen.h",
    ]

    install: true
    installDir: qbs.targetOS.contains("qnx") ? FileInfo.joinPaths("/tmp", name, "bin") : base
}
