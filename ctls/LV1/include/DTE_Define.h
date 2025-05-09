//#define DTE_FUNCTION_ENABLE			1

#define DTE_ANALOGUE_CARRIER_ON		0xA0U
#define DTE_ANALOGUE_CARRIER_OFF	0xA1U
#define DTE_ANALOGUE_POLLING		0xA2U
#define DTE_ANALOGUE_RESET			0xA3U
#define DTE_ANALOGUE_WUPA			0xA4U
#define DTE_ANALOGUE_WUPB			0xA5U
#define DTE_ANALOGUE_RATS			0xA6U
#define DTE_ANALOGUE_ATTRIB			0xA7U
#define DTE_PREVALIDATION_START		0xA8U
#define DTE_PREVALIDATION_STOP		0xA9U
#define DTE_LOOPBACK_START			0xAAU
#define DTE_LOOPBACK_STOP			0xABU
#define DTE_SENDAPPLICATION_START	0xACU
#define DTE_SENDAPPLICATION_STOP	0xADU
#define DTE_UPLOAD_DATA				0xC0U
#define DTE_LOADMODULATION_START	0xC1U
#define DTE_LOADMODULATION_STOP		0xC2U

#define DTE_WRITE_REGISTER			0xD0U
#define DTE_WRITE_REGISTER_RxAna_A	0xD1U
#define DTE_WRITE_REGISTER_RxAna_B	0xD2U
#define DTE_WRITE_REGISTER_TxAmp_A	0xD3U
#define DTE_WRITE_REGISTER_TxAmp_B	0xD4U
#define DTE_WRITE_REGISTER_TxAmp_C	0xD5U

#define DTE_READ_REGISTER			0xE0U
#define DTE_READ_REGISTER_RxAna_A	0xE1U
#define DTE_READ_REGISTER_RxAna_B	0xE2U
#define DTE_READ_REGISTER_TxAmp_A	0xE3U
#define DTE_READ_REGISTER_TxAmp_B	0xE4U

#define DTE_SEND_Buffer_SIZE		2048
#define DTE_RECEIVE_Buffer_SIZE		2048

#define DTE_MODE_PERIPHERAL_AUX		0
#define DTE_MODE_PERIPHERAL_USB		1
