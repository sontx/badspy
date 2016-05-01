' argument for this file: {zip file} [extract folder]

Call WScript.Sleep(1000)

Set shell = CreateObject("WScript.Shell")
Set fso = CreateObject("Scripting.FileSystemObject")

zipFile = WScript.Arguments.Item(0)
Dim folder

If WScript.Arguments.Count = 1 Then' do not define extract folder, so using current folder to do this
	tmpName = fso.GetTempName
	folder = shell.CurrentDirectory + "\" + tmpName
Else
	folder = WScript.Arguments.Item(1)
End If

If Not fso.FolderExists(folder) Then
	fso.CreateFolder(folder)
End If

Set objShell = CreateObject("Shell.Application")
Set filesInZip=objShell.NameSpace(zipFile).items
Call objShell.NameSpace(folder).CopyHere(filesInZip, 3092)

Call fso.DeleteFile(zipFile)

installFile = fso.BuildPath(folder, "install.bat")
If fso.FileExists(installFile) Then
	Call shell.Run(Chr(34) & installFile & Chr(34), 1, False)
End If

strScript = Wscript.ScriptFullName
fso.DeleteFile(strScript)

Set objShell = Nothing
Set fso = Nothing
Set shell = Nothing