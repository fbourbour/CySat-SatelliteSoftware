; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDakotaAlphaSimView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DakotaAlphaSim.h"
LastPage=0

ClassCount=5
Class1=CDakotaAlphaSimApp
Class2=CDakotaAlphaSimDoc
Class3=CDakotaAlphaSimView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource4=IDR_CNTR_INPLACE
Class5=CAboutDlg
Resource7=IDD_DAKOTAALPHASIM_FORM

[CLS:CDakotaAlphaSimApp]
Type=0
HeaderFile=DakotaAlphaSim.h
ImplementationFile=DakotaAlphaSim.cpp
Filter=N

[CLS:CDakotaAlphaSimDoc]
Type=0
HeaderFile=DakotaAlphaSimDoc.h
ImplementationFile=DakotaAlphaSimDoc.cpp
Filter=N

[CLS:CDakotaAlphaSimView]
Type=0
HeaderFile=DakotaAlphaSimView.h
ImplementationFile=DakotaAlphaSimView.cpp
Filter=D
BaseClass=CFormView
VirtualFilter=VWC
LastObject=CDakotaAlphaSimView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T




[CLS:CAboutDlg]
Type=0
HeaderFile=DakotaAlphaSim.cpp
ImplementationFile=DakotaAlphaSim.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_EDIT_PASTE_SPECIAL
Command15=ID_OLE_INSERT_NEW
Command16=ID_OLE_EDIT_LINKS
Command17=ID_OLE_VERB_FIRST
Command18=ID_VIEW_TOOLBAR
Command19=ID_VIEW_STATUS_BAR
Command20=ID_APP_ABOUT
CommandCount=20

[MNU:IDR_CNTR_INPLACE]
Type=1
Class=CDakotaAlphaSimView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
CommandCount=9

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
Command15=ID_CANCEL_EDIT_CNTR
CommandCount=15

[ACL:IDR_CNTR_INPLACE]
Type=1
Class=CDakotaAlphaSimView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_CANCEL_EDIT_CNTR
CommandCount=7

[DLG:IDD_DAKOTAALPHASIM_FORM]
Type=1
Class=CDakotaAlphaSimView
ControlCount=41
Control1=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control2=IDC_STATIC,button,1342177287
Control3=IDC_EDIT_TEMPERATURE,edit,1350631552
Control4=IDC_EDIT_POWER,edit,1350631552
Control5=IDC_EDIT_ATTITUDE,edit,1350631552
Control6=IDC_EDIT_ORBIT,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_BUTTON1,button,1342242816
Control13=IDC_STATIC,button,1342177287
Control14=IDC_RADIO_ON,button,1342177289
Control15=IDC_RADIO_OFF,button,1342177289
Control16=IDC_EDIT_BEACON_INTERVAL,edit,1350631552
Control17=IDC_STATIC,static,1342308352
Control18=IDC_EDIT_BEACON_INTERVAL_COUNTER,edit,1350633600
Control19=IDC_STATIC,static,1342308352
Control20=IDC_CHECK_SENT,button,1342242819
Control21=IDC_EDIT_MESSAGE,edit,1350633600
Control22=IDC_STATIC,static,1342308352
Control23=IDC_BUTTON_SEND_IMAGE,button,1342242816
Control24=IDC_STATIC,button,1342177287
Control25=IDC_TRANSMITTER_OFF,button,1342242819
Control26=IDC_TRANSMITTER_ON,button,1342242819
Control27=IDC_STATIC,button,1342177287
Control28=IDC_EDIT_X_A,edit,1350631552
Control29=IDC_EDIT_Y_A,edit,1350631552
Control30=IDC_EDIT_Z_A,edit,1350631552
Control31=IDC_STATIC,static,1342308352
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_EDIT_X_AXIS_M,edit,1350631552
Control36=IDC_EDITY_AXIS_M,edit,1350631552
Control37=IDC_EDIT_Z_AXIS_M,edit,1350631552
Control38=IDC_STATIC,static,1342308352
Control39=IDC_STATIC,static,1342308352
Control40=IDC_STATIC,static,1342308352
Control41=IDC_STATIC,static,1342308352

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

