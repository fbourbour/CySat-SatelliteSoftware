// DakotaAlphaSimView.cpp : implementation of the CDakotaAlphaSimView class
//

#include "stdafx.h"



#include "DakotaAlphaSim.h"

#include "DakotaAlphaSimDoc.h"
#include "CntrItem.h"
#include "DakotaAlphaSimView.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ON 1
#define OFF 0

#define DATA_REQUEST 0xFF
#define IMAGE_REQUEST 0xFE
#define STANDBY_REQUEST 0xFC

heliumPacket DakotaAlphaPacket;


CString HeliumData;

const char pcHelloPacket[] = {
        0x48,0x65,0x10,0x03,0x00,0x0e,0x21,0x57,0x48,0x65,0x6c,0x6c,
        0x6f,0x2c,0x20,0x77,0x6f,0x72,0x6c,0x64,0x21,0x0d,0x2f,0x77
    };
/****************************************************************************

Rev1.0  
Programmer: Fred Bourbour


*****************************************************************************/
// CDakotaAlphaSimView

IMPLEMENT_DYNCREATE(CDakotaAlphaSimView, CFormView)

BEGIN_MESSAGE_MAP(CDakotaAlphaSimView, CFormView)
	//{{AFX_MSG_MAP(CDakotaAlphaSimView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, OnCancelEditCntr)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonSend)
	ON_BN_CLICKED(IDC_RADIO_ON, OnBeaconOn)
	ON_BN_CLICKED(IDC_RADIO_OFF, OnBeaconOff)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SEND_IMAGE, OnButtonSendImage)
	ON_BN_CLICKED(IDC_TRANSMITTER_OFF, OnTransmitterOff)
	ON_BN_CLICKED(IDC_TRANSMITTER_ON, OnTransmitterOn)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDakotaAlphaSimView construction/destruction

CDakotaAlphaSimView::CDakotaAlphaSimView()
	: CFormView(CDakotaAlphaSimView::IDD)
{
	//{{AFX_DATA_INIT(CDakotaAlphaSimView)
	m_BeaconInterval = 0;
	m_BeaconIntervalCounter = 0;
	m_BeaconSent = FALSE;
	m_Attitude = _T("");
	m_Orbit = _T("");
	m_Power = _T("");
	m_Temperature = _T("");
	m_Message = _T("");
	m_TransmitterOff = FALSE;
	m_TransmitterOn = FALSE;
	m_XaxisAcc = _T("");
	m_YaxisAcc = _T("");
	m_ZaxisAcc = _T("");
	m_XaxisMag = _T("");
	m_YaxisMag = _T("");
	m_ZaxisMag = _T("");
	//}}AFX_DATA_INIT
	m_pSelection = NULL;
	// TODO: add construction code here

	m_Temperature = "-20";
	m_Power = "3.9";
	m_Attitude = "60";
	m_Orbit = "400";
	
	m_XaxisAcc =  
	m_XaxisMag = "11";
	m_YaxisAcc = 
	m_YaxisMag = "12";
	m_ZaxisAcc = 
	m_ZaxisMag = "13";
	

}

CDakotaAlphaSimView::~CDakotaAlphaSimView()
{
}

void CDakotaAlphaSimView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDakotaAlphaSimView)
	DDX_Control(pDX, IDC_MSCOMM1, m_comm1);
	DDX_Text(pDX, IDC_EDIT_BEACON_INTERVAL, m_BeaconInterval);
	DDX_Text(pDX, IDC_EDIT_BEACON_INTERVAL_COUNTER, m_BeaconIntervalCounter);
	DDX_Check(pDX, IDC_CHECK_SENT, m_BeaconSent);
	DDX_Text(pDX, IDC_EDIT_ATTITUDE, m_Attitude);
	DDX_Text(pDX, IDC_EDIT_ORBIT, m_Orbit);
	DDX_Text(pDX, IDC_EDIT_POWER, m_Power);
	DDX_Text(pDX, IDC_EDIT_TEMPERATURE, m_Temperature);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_Message);
	DDX_Check(pDX, IDC_TRANSMITTER_OFF, m_TransmitterOff);
	DDX_Check(pDX, IDC_TRANSMITTER_ON, m_TransmitterOn);
	DDX_Text(pDX, IDC_EDIT_X_A, m_XaxisAcc);
	DDX_Text(pDX, IDC_EDIT_Y_A, m_YaxisAcc);
	DDX_Text(pDX, IDC_EDIT_Z_A, m_ZaxisAcc);
	DDX_Text(pDX, IDC_EDIT_X_AXIS_M, m_XaxisMag);
	DDX_Text(pDX, IDC_EDITY_AXIS_M, m_YaxisMag);
	DDX_Text(pDX, IDC_EDIT_Z_AXIS_M, m_ZaxisMag);
	//}}AFX_DATA_MAP
}

BOOL CDakotaAlphaSimView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CDakotaAlphaSimView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();


	// TODO: remove this code when final selection model code is written
	m_pSelection = NULL;    // initialize selection

}

/////////////////////////////////////////////////////////////////////////////
// CDakotaAlphaSimView printing

BOOL CDakotaAlphaSimView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDakotaAlphaSimView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDakotaAlphaSimView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDakotaAlphaSimView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

void CDakotaAlphaSimView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
   CFormView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
}


/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

BOOL CDakotaAlphaSimView::IsSelected(const CObject* pDocItem) const
{
	// The implementation below is adequate if your selection consists of
	//  only CDakotaAlphaSimCntrItem objects.  To handle different selection
	//  mechanisms, the implementation here should be replaced.

	// TODO: implement this function that tests for a selected OLE client item

	return pDocItem == m_pSelection;
}

void CDakotaAlphaSimView::OnInsertObject()
{
	// Invoke the standard Insert Object dialog box to obtain information
	//  for new CDakotaAlphaSimCntrItem object.
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	CDakotaAlphaSimCntrItem* pItem = NULL;
	TRY
	{
		// Create new item connected to this document.
		CDakotaAlphaSimDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pItem = new CDakotaAlphaSimCntrItem(pDoc);
		ASSERT_VALID(pItem);

		// Initialize the item from the dialog data.
		if (!dlg.CreateItem(pItem))
			AfxThrowMemoryException();  // any exception will do
		ASSERT_VALID(pItem);
		
        if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);

		ASSERT_VALID(pItem);

		// As an arbitrary user interface design, this sets the selection
		//  to the last item inserted.

		// TODO: reimplement selection as appropriate for your application

		m_pSelection = pItem;   // set selection to last inserted item
		pDoc->UpdateAllViews(NULL);
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH

	EndWaitCursor();
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the container (not the server) causes the deactivation.
void CDakotaAlphaSimView::OnCancelEditCntr()
{
	// Close any in-place active item on this view.
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
void CDakotaAlphaSimView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // don't call the base class
			return;
		}
	}

	CFormView::OnSetFocus(pOldWnd);
}

void CDakotaAlphaSimView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();
}

/////////////////////////////////////////////////////////////////////////////
// CDakotaAlphaSimView diagnostics

#ifdef _DEBUG
void CDakotaAlphaSimView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDakotaAlphaSimView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDakotaAlphaSimDoc* CDakotaAlphaSimView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDakotaAlphaSimDoc)));
	return (CDakotaAlphaSimDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDakotaAlphaSimView message handlers

BEGIN_EVENTSINK_MAP(CDakotaAlphaSimView, CFormView)
    //{{AFX_EVENTSINK_MAP(CDakotaAlphaSimView)
	ON_EVENT(CDakotaAlphaSimView, IDC_MSCOMM1, 1 /* OnComm */, OnCom1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDakotaAlphaSimView::OnCom1() 
{
int length;
unsigned char ch,i;

CString InputString;
VARIANT Value;
Value.vt = VT_BSTR;
Value = m_comm1.GetInput();
InputString = Value.bstrVal;
length = InputString.GetLength();
if(!length)
return;

 for(i = 0; i<length; i++)
 {
   ch = InputString.GetAt(i);
   ProcessGroundStationCommand(ch);
 }	
}

void CDakotaAlphaSimView::ProcessGroundStationCommand(char ch)
{
static int byte_counter = 0;
int byte_counter_save;
static uint8_t rcvBuffer[ PACKET_MAX_SIZE ];
uint8_t pucBuffer[PACKET_MAX_SIZE];

memset(pucBuffer,0x00, sizeof(pucBuffer));

int i,j;

i = byte_counter; //for debug

 rcvBuffer[byte_counter] = (uint8_t) ch;

 if(ch == SYNC_1)
  byte_counter = 0;

 if( byte_counter == 0 )
  {
        DakotaAlphaPacket.pucPayload = NULL;
  }


if((byte_counter == 0 && ch != SYNC_1) || 
   (byte_counter == 1 && ch != SYNC_2) || //look for sync characters
   (byte_counter == 2 && ch != MESSAGE_I && ch != MESSAGE_O) ||
   (byte_counter == 3 && ch == 0))
{
byte_counter = 0;
return;
}


	byte_counter_save = byte_counter;

switch (byte_counter)
{
    case 0:
    case 1:
    case 2:
    case 3:
    break;


    case HEADER_SIZE-1:
          ParseHeader( rcvBuffer, &DakotaAlphaPacket);

		  for(j = 0; j<HEADER_SIZE + DakotaAlphaPacket.usPayloadSize + 1; j++) 
            pucBuffer[j] = rcvBuffer[j];

           if( DakotaAlphaPacket.ucCommand == 0)
		   {
            /* Checksum failed */
            /* TODO Send NACK? */
            byte_counter = 0;
			m_Message = "Header Checksum Error";
			UpdateData(FALSE);
            return;
		   }

		   if(0)// DakotaAlphaPacket.usAck != 0x30)
		   {
            /* received ACK or NACK */
			m_Message = "Received Ack or Nack";
            byte_counter = 0;
			UpdateData(FALSE);
            return;
		   }


     break;


	default:
	
		if(byte_counter >= HEADER_SIZE)
		{

        if( HEADER_SIZE + DakotaAlphaPacket.usPayloadSize + 1 == byte_counter )
        {

			for(j = 0; j<HEADER_SIZE + DakotaAlphaPacket.usPayloadSize + 1; j++) 
            pucBuffer[j] = rcvBuffer[j];

            uint8_t ucSumA, ucSumB;
            vCalculateChecksum( &rcvBuffer[2], HEADER_SIZE - 2 + DakotaAlphaPacket.usPayloadSize, &ucSumA, &ucSumB );

            uint8_t A = rcvBuffer[byte_counter-1];
            uint8_t B = rcvBuffer[byte_counter];

            if(0) //( ucSumA != rcvBuffer[byte_counter-1] || ucSumB != rcvBuffer[byte_counter] )
            {
                /* Checksum failed */
				m_Message = "Payload Checksum Failed";
				UpdateData(FALSE);
                byte_counter = 0;
                return;
            }
			else
			{
				//m_Message = "Payload Successfully Received";
				//UpdateData(FALSE);
				pucBuffer[HEADER_SIZE + DakotaAlphaPacket.usPayloadSize] = '\0'; 
				ProcessRequest((char*)&pucBuffer[HEADER_SIZE]);
			}


            /* TODO Copy buffer to DakotaAlphaPacket, and give to receive queue */
        }


		}


}

byte_counter++;

}




void CDakotaAlphaSimView::ParseHeader( uint8_t *pucData, heliumPacket *DakotaAlphaPacket )
{
    uint8_t ucSumA, ucSumB;
	char temp[2];

    uint8_t A,B;

    vCalculateChecksum( pucData+2, 4, &ucSumA, &ucSumB );
    if( ucSumA != pucData[6] || ucSumB != pucData[7] )
    {
        DakotaAlphaPacket->ucCommand     = 0;
        DakotaAlphaPacket->usPayloadSize = 0;
        DakotaAlphaPacket->usAck         = 0;
        return;
    }

    /* Ignore I/O field (pucData[2]) */
	if(pucData[3] >= 0x30 && pucData[3] <= 0x39)
     DakotaAlphaPacket->ucCommand     = pucData[3] - 0x30;      
    else
     DakotaAlphaPacket->ucCommand     = pucData[3];

if(pucData[4] >= 0x30 && pucData[4] <= 0x39)
{
	temp[0] = pucData[4];
	temp[1] ='\0';
	A = atoi(temp);
	
}
else
    A = pucData[4];
	
if(pucData[5] >= 0x30 && pucData[5] <= 0x39)
{
	temp[0] = pucData[5];
	temp[1] ='\0';
	B = atoi(temp);
	
}
else
    B = pucData[5];
    

	DakotaAlphaPacket->usPayloadSize = (A << 8) | (0xFF & B);

    //DakotaAlphaPacket->usPayloadSize = (A << 8) | (0xFF & B);

    if( DakotaAlphaPacket->usPayloadSize > PAYLOAD_MAX_SIZE )
    {
        DakotaAlphaPacket->usAck         = DakotaAlphaPacket->usPayloadSize;
        DakotaAlphaPacket->usPayloadSize = 0;
    }
    else
    {
        DakotaAlphaPacket->usAck         = 0;
    }
}

void CDakotaAlphaSimView::vCalculateChecksum( uint8_t *pucData, uint16_t usSize, uint8_t *pucSumA, uint8_t *pucSumB )
{
    uint16_t i;
	char test;

    *pucSumA = 0;
    *pucSumB = 0;

    for( i = 0; i < usSize; i++ )
    {
		test = pucData[i];
		//if(test == 0)
		//	continue;

        *pucSumA += pucData[i];
        *pucSumB += *pucSumA;
    }
}


void CDakotaAlphaSimView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
static char once = 0;	
CDocument *pDoc = GetDocument();

pDoc->SetTitle("UND-ISU CySat CubeSatSim 1.00");	
	// Do not call CFormView::OnPaint() for painting messages

if(once == 0)
{
once = 1;
m_comm1.SetPortOpen(1);

m_TransmitterOn = false;
m_TransmitterOff = true;
UpdateData(FALSE);

}

}
void CDakotaAlphaSimView::ProcessRequest(char* pucBuffer)
{
unsigned char request;

request = atoi(pucBuffer);
if( request== DATA_REQUEST)
{
 
   UpdateData(TRUE);
   OnButtonSend();
   m_Message ="Data Request";
   UpdateData(FALSE);
}
if(request == IMAGE_REQUEST)
{
   m_Message ="Image Request";
   UpdateData(FALSE);
   OnButtonSendImage();
}

}



void CDakotaAlphaSimView::OnButtonSend() 
{

UpdateData(TRUE);
SendCommandToGroundStation(m_Temperature + "," + m_Power + "," + m_Attitude + "," + m_Orbit 
						                 + "," + m_XaxisAcc + "," + m_YaxisAcc + "," +
										 m_ZaxisAcc + "," + m_XaxisMag + "," + m_YaxisMag + "," +
										 m_ZaxisMag);
	
}

void CDakotaAlphaSimView::TransmitToGroundStation(CString data_out)
{

    VARIANT Value;
    const VARIANT &ValueRef = Value;
    ::VariantInit(&Value);
    Value.vt = VT_BSTR;
    Value.bstrVal = data_out.AllocSysString();
    m_comm1.SetOutput(ValueRef);
    ::VariantClear(&Value);


}



void CDakotaAlphaSimView::OnBeaconOn() 
{
UpdateData(TRUE);
bBeaconStatus = ON;
if(m_BeaconInterval == 0)
m_BeaconInterval = 18;
m_BeaconIntervalCounter = m_BeaconInterval;
SetTimer(1,1000,NULL);
UpdateData(FALSE);	
}

void CDakotaAlphaSimView::OnBeaconOff() 
{
	bBeaconStatus = OFF;
	KillTimer(1);
	
}

void CDakotaAlphaSimView::OnTimer(UINT nIDEvent) 
{

	if(--m_BeaconIntervalCounter == 0)
	{
	 UpdateData(TRUE);
     m_BeaconIntervalCounter = m_BeaconInterval;
	 m_BeaconSent = ON;
	 SendCommandToGroundStation(m_Temperature + m_Power + m_Attitude + m_Orbit);
	}
	else
	m_BeaconSent = OFF;

	UpdateData(FALSE);

	CFormView::OnTimer(nIDEvent);
}

void CDakotaAlphaSimView::SendCommandToGroundStation(CString house_keeping)
{


CString Buffer ="";
/*
    uint8_t i;
	CString data_Out = "00";
    data_Out.SetAt(0,'%');
	for(i = 0;i < house_keeping.GetLength(); i++)
    data_Out.SetAt(1,house_keeping.GetAt(i));
*/


    DakotaAlphaPacket.ucCommand = COMMAND_TRANSMIT;
    DakotaAlphaPacket.usPayloadSize = house_keeping.GetLength();
    DakotaAlphaPacket.pucPayload = (uint8_t*)house_keeping.GetBuffer(house_keeping.GetLength());
    DakotaAlphaPacket.usAck = 0;


	vHeliumSendPacket(&DakotaAlphaPacket);

	//Buffer += (char*)vHeliumSendPacket(&DakotaAlphaPacket);
    TransmitToGroundStation(HeliumData);
}

void CDakotaAlphaSimView::OnButtonSendImage() 
{
FILE* image_file;
FILE* copy_file;
char data;
long file_size,file_size2,i;
char data_buffer[255];
//if((image_file = fopen("c:\\cubesat\\DakotaAlphaSim\\pink.bmp", "r")) == NULL)
if((image_file = fopen("c:\\cubesat\\DakotaAlphaSim\\hello.txt", "r")) == NULL)
{
 
 m_Message = "Failed to open the image file";
 UpdateData(FALSE);
 return;
}

//copy_file = fopen("c:\\cubesat\\DakotaAlphaSim\\copy_image.bmp", "w");
copy_file = fopen("c:\\cubesat\\DakotaAlphaSim\\copy_image.txt", "w");

DakotaAlphaPacket.ucCommand = COMMAND_TRANSMIT;

fseek(image_file,0L,SEEK_END);
file_size = ftell(image_file);

m_TransmitterOn = true;
m_TransmitterOff = false;
UpdateData(FALSE);

fseek(image_file,0L,SEEK_SET);

for(i = 0; i < file_size; i+=PAYLOAD_MAX_SIZE)
{
 
fseek(image_file,i,SEEK_SET);

fread(data_buffer,1,PAYLOAD_MAX_SIZE,image_file); 
fwrite(data_buffer,1,PAYLOAD_MAX_SIZE,copy_file);

//fread(&data,1,1,image_file); 
//fwrite(&data,1,1,copy_file);
    
    DakotaAlphaPacket.usPayloadSize = PAYLOAD_MAX_SIZE;
    DakotaAlphaPacket.pucPayload = (unsigned char*)data_buffer;
    DakotaAlphaPacket.usAck = 0;
    vHeliumSendPacket(&DakotaAlphaPacket);
    TransmitToGroundStation(HeliumData);;
	
}

fseek(copy_file,0L,SEEK_END);
file_size2 = ftell(copy_file);


fclose(image_file);
fclose(copy_file);	

m_TransmitterOn = false;
m_TransmitterOff = true;
UpdateData(FALSE);

}

void CDakotaAlphaSimView::OnTransmitterOff() 
{
	// TODO: Add your control notification handler code here
	
}

void CDakotaAlphaSimView::OnTransmitterOn() 
{
	// TODO: Add your control notification handler code here
	
}
