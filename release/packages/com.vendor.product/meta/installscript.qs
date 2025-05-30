function Component() {}

Component.prototype.createOperations = function() {
    // Appelle les opérations par défaut
    component.createOperations();

    // Créer un raccourci sur le bureau vers Tetris.exe
    var targetPath = "@TargetDir@/Tetris.exe";
    var shortcutPath = "@DesktopDir@/Tetris.lnk";
    var workingDirectory = "@TargetDir@";

    component.addOperation("CreateShortcut",
        targetPath, shortcutPath,
        "workingDirectory=" + workingDirectory,
		"iconPath=C:/Users/fpier/Documents/QtCreator/tetris/tetris.ico"
    );

}
