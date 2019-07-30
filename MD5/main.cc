#define _CRT_SECURE_NO_WARNINGS 1
#include"UIHead.h"
#include"MD5.h"

class CDuiFrameWnd : public WindowImplBase {
	private:
	CDuiString FilePath ;
	public:
	virtual LPCTSTR    GetWindowClassName () const {
		return _T ("DUIMainFrame");
	}
	virtual CDuiString GetSkinFile () {
		return _T ("duilib.xml");
	}
	virtual CDuiString GetSkinFolder () {
		return _T ("");
	}
	virtual void Notify (TNotifyUI& msg) {
		CDuiString strControlName = msg.pSender->GetName ();
		if ( msg.sType == _T ("click") ) {
			if ( strControlName == _T ("XuanZe") ) {//��ȡ�ռ�����
				LoadFile ();
			}
			else if ( strControlName == _T ("ShengCheng") ) {
				GetFileMD5 ();
			}
			else 
				OnClick (msg);//��С�� ���//�رյ�Ĭ��ʵ�֣�
		}
	}
	void LoadFile () {
		//MessageBox (NULL , _T ("����") , _T ("������") , IDOK);
		OPENFILENAME ofn;//�˽�ṹ��
		TCHAR FileName [MAX_PATH];//���ַ�
		memset (&ofn , 0 , sizeof (OPENFILENAME));
		memset (FileName , 0 , sizeof (char)*MAX_PATH);
		ofn.lStructSize = sizeof (OPENFILENAME);
		ofn.lpstrFilter = _T ("\0");
		ofn.lpstrFile = FileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_FILEMUSTEXIST;
		//bool once= true;
		if ( GetOpenFileName (&ofn) )//FileName�õ��û���ѡ���ļ���·�� 
		{
			//MessageBox (NULL , FileName , NULL , NULL);
			CEditUI* pPathEdit = (CEditUI*)m_PaintManager.FindControl (_T ("WBK1"));
			pPathEdit->SetText (FileName);
			FilePath = pPathEdit->GetText ();
			//once = false;
		}
	}
	void GetFileMD5 () {
		MD5 md5;
		string Path = StringFromLPCTSTR (FilePath.GetData ()); 
		string result = md5.getFileMd5 (Path.c_str ());
		wstring MD5Code = A2W (result);
		CEditUI* pPathEdit = (CEditUI*)m_PaintManager.FindControl (_T ("WBK2"));
		pPathEdit->SetText (MD5Code.c_str());
	}
	void Clipboard () {
		//SetClipboardData ();
	}

	std::string StringFromLPCTSTR (LPCTSTR str) {//CDuiString��string��
		int size_str = WideCharToMultiByte (CP_ACP , 0 , str , -1 , 0 , 0 , NULL , NULL);//unft-8������һֱ�ǣ�����
		char* point_new_array = new char [size_str];
		WideCharToMultiByte (CP_ACP , 0 , str , -1 , point_new_array , size_str , NULL , NULL);
		std::string return_string (point_new_array);
		delete[] point_new_array;
		point_new_array = NULL;
		return return_string;
	}
	wstring A2W (const string &s) {//�ַ��������ַ�����
		const char * szSource = s.c_str ();
		size_t _Dsize = MultiByteToWideChar (CP_ACP , 0 , szSource , -1 , NULL , 0);
		wchar_t * wszDest = new wchar_t [_Dsize];
		MultiByteToWideChar (CP_ACP , 0 , szSource , -1 , wszDest , _Dsize);
		wstring result = wszDest;
		delete[]wszDest;
		return result;
	}
};

int APIENTRY _tWinMain (HINSTANCE hInstance , HINSTANCE hPrevInstance , LPTSTR lpCmdLine , int nCmdShow) {
	CPaintManagerUI::SetInstance (hInstance);

	CDuiFrameWnd duiFrame;
	duiFrame.Create (NULL , _T ("DUIWnd") , UI_WNDSTYLE_FRAME , WS_EX_WINDOWEDGE);
	duiFrame.CenterWindow ();
	duiFrame.ShowModal ();
	return 0;
}