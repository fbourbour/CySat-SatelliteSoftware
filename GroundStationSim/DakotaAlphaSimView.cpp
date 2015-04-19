// DakotaAlphaSimView.cpp : implementation of the CDakotaAlphaSimView class
//

#include "stdafx.h"
#include "DakotaAlphaSim.h"

#include "DakotaAlphaSimDoc.h"
#include "CntrItem.h"
#include "DakotaAlphaSimView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ON 1
#define OFF 0

#define NO_MODE 1
#define NORMAL_MODE 2
#define IMAGE_MODE 3
#define CONSTELLATION_MODE 4
#define SHUT_DOWN_MODE 5


#define DATA_REQUEST 0xFF
#define IMAGE_REQUEST 0xFE

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
	ON_BN_CLICKED(IDC_RADIO_ON, OnBeaconOn)
	ON_BN_CLICKED(IDC_RADIO_OFF, OnBeaconOff)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_IMAGE, OnButtonImage)
	ON_BN_CLICKED(IDC_IMAGE_MODE, OnImageMode)
	ON_BN_CLICKED(IDC_RADIO_NORMAL, OnRadioNormal)
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
	m_Message = _T("");
	m_Temperature = _T("");
	m_Power = _T("");
	m_Attitude = _T("");
	m_Orbit = _T("");
	m_XaxisAcc = _T("");
	m_XaxisMag = _T("");
	m_YaxisAcc = _T("");
	m_YaxisMag = _T("");
	m_ZaxisAcc = _T("");
	m_ZaxisMag = _T("");
	//}}AFX_DATA_INIT
	m_pSelection = NULL;
	// TODO: add construction code here

	image_file = NULL;
	m_Mode = NO_MODE;

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
	DDX_Text(pDX, IDC_MESSAGE, m_Message);
	DDX_Text(pDX, IDC_EDIT1, m_Temperature);
	DDV_MaxChars(pDX, m_Temperature, 10);
	DDX_Text(pDX, IDC_EDIT2, m_Power);
	DDX_Text(pDX, IDC_EDIT3, m_Attitude);
	DDX_Text(pDX, IDC_EDIT5, m_Orbit);
	DDX_Text(pDX, IDC_EDIT_X_A, m_XaxisAcc);
	DDX_Text(pDX, IDC_EDIT_X_M, m_XaxisMag);
	DDX_Text(pDX, IDC_EDIT_Y_A, m_YaxisAcc);
	DDX_Text(pDX, IDC_EDIT_Y_M, m_YaxisMag);
	DDX_Text(pDX, IDC_EDIT_Z_A, m_ZaxisAcc);
	DDX_Text(pDX, IDC_EDIT1_Z_M, m_ZaxisMag);
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

m_Message = "";
UpdateData(FALSE);

 for(i = 0; i<length; i++)
 {
   ch = InputString.GetAt(i);
   //if(ch >= 0x30 && ch <= 0x39)
   //	   ch -= 0x30;
   ProcessCubeSatData(ch);
 }	
}



void CDakotaAlphaSimView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
static char once = 0;	
CDocument *pDoc = GetDocument();

pDoc->SetTitle("UND-ISU CySat GroundStationSim 1.00");	
	// Do not call CFormView::OnPaint() for painting messages

if(once == 0)
{
once = 1;
m_comm1.SetPortOpen(1);

}

}

void CDakotaAlphaSimView::TransmitToCubeSat(CString data_out)
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


	CFormView::OnTimer(nIDEvent);
}

void CDakotaAlphaSimView::SendRequestToCubeSat(char request)
{


CString buffer ="";
/*
    uint8_t i;
	CString data_Out = "00";
    data_Out.SetAt(0,'%');
	for(i = 0;i < command.GetLength(); i++)
    data_Out.SetAt(1,command.GetAt(i));
*/
    
    buffer.Format("%d",request);

    DakotaAlphaPacket.ucCommand = COMMAND_TRANSMIT;
    DakotaAlphaPacket.usPayloadSize = buffer.GetLength();
    DakotaAlphaPacket.pucPayload = (uint8_t*)buffer.GetBuffer(buffer.GetLength());
    DakotaAlphaPacket.usAck = 0;


	vHeliumSendPacket(&DakotaAlphaPacket);

	//buffer += (char*)vHeliumSendPacket(&DakotaAlphaPacket);
    TransmitToCubeSat(HeliumData);
}




void CDakotaAlphaSimView::ProcessCubeSatData(char ch)
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
				m_Message = "Payload Successfully Received";
				UpdateData(FALSE);
				pucBuffer[HEADER_SIZE + DakotaAlphaPacket.usPayloadSize] = '\0'; 
				ProcessPayload((char*)&pucBuffer[HEADER_SIZE]);
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
void CDakotaAlphaSimView::ProcessPayload(char* pucBuffer)
{
CString data;
int length;

length = strlen(pucBuffer);

unsigned short i;

if(m_Mode == IMAGE_MODE)
{
if(image_file == NULL)
return;

//fwrite(pucBuffer,sizeof(char),PAYLOAD_MAX_SIZE,image_file);
fwrite(pucBuffer,sizeof(char),strlen(pucBuffer),image_file);

return;
}
else if (m_Mode != NORMAL_MODE)
return;


//sscanf(pucBuffer,"%s,%s,%s,%s",m_Temperature,m_Power,m_Attitude,m_Orbit);



//Temperature
data = pucBuffer;
i = data.Find(",");
data.SetAt(i,0);

m_Temperature = data;




//Power
data = pucBuffer;
data.TrimLeft(m_Temperature);
data.TrimLeft(",");
i = data.Find(",");
data.SetAt(i,0);
m_Power = data;

//Attitude

data = pucBuffer;
data.TrimLeft(m_Temperature);
data.TrimLeft(",");
data.TrimLeft(m_Power);
data.TrimLeft(",");
i = data.Find(",");
data.SetAt(i,0);

m_Attitude = data;


//Orbit

data = pucBuffer;
data.TrimLeft(m_Temperature);
data.TrimLeft(",");
data.TrimLeft(m_Power);
data.TrimLeft(",");
data.TrimLeft(m_Attitude);
data.TrimLeft(",");
i = data.Find(",");
data.SetAt(i,0);

m_Orbit = data;

data = pucBuffer;
data.TrimLeft(m_Temperature);
data.TrimLeft(",");
data.TrimLeft(m_Power);
data.TrimLeft(",");
data.TrimLeft(m_Attitude);
data.TrimLeft(",");
data.TrimLeft(m_Orbit);
data.TrimLeft(",");

i = data.Find(",");
data.SetAt(i,0);

m_XaxisAcc = data;


data = pucBuffer;
data.TrimLeft(m_Temperature);
data.TrimLeft(",");
data.TrimLeft(m_Power);
data.TrimLeft(",");
data.TrimLeft(m_Attitude);
data.TrimLeft(",");
data.TrimLeft(m_Orbit);
data.TrimLeft(",");
data.TrimLeft(m_XaxisAcc);
data.TrimLeft(",");
i = data.Find(",");
data.SetAt(i,0);
m_YaxisAcc = data;


data = pucBuffer;
data.TrimLeft(m_Temperature);
data.TrimLeft(",");
data.TrimLeft(m_Power);
data.TrimLeft(",");
data.TrimLeft(m_Attitude);
data.TrimLeft(",");
data.TrimLeft(m_Orbit);
data.TrimLeft(",");
data.TrimLeft(m_XaxisAcc);
data.TrimLeft(",");
data.TrimLeft(m_YaxisAcc);
data.TrimLeft(",");
i = data.Find(",");
data.SetAt(i,0);
m_ZaxisAcc = data;


data = pucBuffer;
data.TrimLeft(m_Temperature);
data.TrimLeft(",");
data.TrimLeft(m_Power);
data.TrimLeft(",");
data.TrimLeft(m_Attitude);
data.TrimLeft(",");
data.TrimLeft(m_Orbit);
data.TrimLeft(",");
data.TrimLeft(m_XaxisAcc);
data.TrimLeft(",");
data.TrimLeft(m_YaxisAcc);
data.TrimLeft(",");
data.TrimLeft(m_ZaxisAcc);
data.TrimLeft(",");
i = data.Find(",");
data.SetAt(i,0);

m_XaxisMag = data;


data = pucBuffer;
data.TrimLeft(m_Temperature);
data.TrimLeft(",");
data.TrimLeft(m_Power);
data.TrimLeft(",");
data.TrimLeft(m_Attitude);
data.TrimLeft(",");
data.TrimLeft(m_Orbit);
data.TrimLeft(",");
data.TrimLeft(m_XaxisAcc);
data.TrimLeft(",");
data.TrimLeft(m_YaxisAcc);
data.TrimLeft(",");
data.TrimLeft(m_ZaxisAcc);
data.TrimLeft(",");
data.TrimLeft(m_XaxisMag);
data.TrimLeft(",");
i = data.Find(",");
data.SetAt(i,0);

m_YaxisMag = data;


data = pucBuffer;
data.TrimLeft(m_Temperature);
data.TrimLeft(",");
data.TrimLeft(m_Power);
data.TrimLeft(",");
data.TrimLeft(m_Attitude);
data.TrimLeft(",");
data.TrimLeft(m_Orbit);
data.TrimLeft(",");
data.TrimLeft(m_XaxisAcc);
data.TrimLeft(",");
data.TrimLeft(m_YaxisAcc);
data.TrimLeft(",");
data.TrimLeft(m_ZaxisAcc);
data.TrimLeft(",");
data.TrimLeft(m_XaxisMag);
data.TrimLeft(",");
data.TrimLeft(m_YaxisMag);
data.TrimLeft(",");

m_ZaxisMag = data;

UpdateData(FALSE);


}

void CDakotaAlphaSimView::OnButtonImage() 
{
static bool file_open = 0;

if(file_open == 0)
image_file = fopen("c:\\CubeSat\\GroundStationSim\\image.bmp","a");
//image_file = fopen("c:\\CubeSat\\GroundStationSim\\text.txt","a");
else
 fclose(image_file);

file_open ^= 0x01;



}


void CDakotaAlphaSimView::OnImageMode() 
{
m_Mode = IMAGE_MODE;
SendRequestToCubeSat(IMAGE_REQUEST);	
}

void CDakotaAlphaSimView::OnRadioNormal() 
{
m_Mode = NORMAL_MODE;
SendRequestToCubeSat(DATA_REQUEST);	
}
