void  SelectMultiFiles(vector<CString> &files)
{
	BOOL isOpen = TRUE;
	CString defaultDir = _T("");
	CString fileName = _T("");
	CString filter = _T("raw文件(.raw)|*.raw|");
	CFileDialog Filedlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT | OFN_EXPLORER, filter);

	const DWORD numberOfFileNames = 32;//最多允许32个文件
	const DWORD fileNameMaxLength = MAX_PATH + 1;
	const DWORD bufferSize = (numberOfFileNames * fileNameMaxLength) + 1;
	TCHAR* filenamesBuffer = new TCHAR[bufferSize];
	// Initialize beginning and end of buffer.
	filenamesBuffer[0] = NULL;//必须的
	filenamesBuffer[bufferSize - 1] = NULL;

	// Attach buffer to OPENFILENAME member.
	Filedlg.m_ofn.lpstrFile = filenamesBuffer;
	Filedlg.m_ofn.nMaxFile = bufferSize;

	int iCtr = 0;
	if(IDOK == Filedlg.DoModal()) {
		CString aviName;
		POSITION pos = Filedlg.GetStartPosition();
		while(pos != NULL) {
			aviName = Filedlg.GetNextPathName(pos);//返回选定文件文件名// Retrieve file name(s).
			//fileNameArray[iCtr] = aviName;
			files.push_back(aviName);
			iCtr++;
		}
	}

	delete[] filenamesBuffer;//Release file names buffer.

}