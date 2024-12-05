
//Transaction Result
#define AEX_RESULT_SUCCESS															0x00U
#define AEX_RESULT_TERMINATE														0x01U
#define AEX_RESULT_TRY_AGAIN														0x02U
#define AEX_RESULT_TRY_ANOTHER_INTERFACE											0x03U
#define AEX_RESULT_SELECT_NEXT														0x04U
#define AEX_RESULT_DECLINE															0x05U
#define AEX_RESULT_ONLINE_REQUEST													0x06U
#define AEX_RESULT_OFFLINE_APPROVE													0x07U
#define AEX_RESULT_RESTART															0x08U
#define AEX_RESULT_REQUEST_ANOTHER_PAYMENT											0x09U
#define AEX_RESULT_INTERRUPT_ICC													0x0AU
#define AEX_RESULT_INTERRUPT_MAG_STRIPE												0x0BU

//Function Parameter
#define AEX_FUNCTION_PARAMETER_GO_ONLINE											0x01U
#define AEX_FUNCTION_PARAMETER_DELAYED_AUTHORIZATION								0x02U

//Transaction Mode
#define AEX_MODE_TRANSACTION_MAGSTRIPE												0x00U
#define AEX_MODE_TRANSACTION_EMV													0x01U

//Interface Mode
#define AEX_MODE_INTERFACE_CARD														0x00U
#define AEX_MODE_INTERFACE_MOBILE													0x01U

//ODA Mode
#define AEX_MODE_ODA_NONE															0x00U
#define AEX_MODE_ODA_SDA															0x01U
#define AEX_MODE_ODA_CDA															0x02U

//DRL Sets
#define AEX_DRL_NUMBER																16

//DRL Flags Combination
#define AEX_DRL_ENABLE_TRANSACTION_LIMIT											0x10U
#define AEX_DRL_ENABLE_CVM_LIMIT													0x08U
#define AEX_DRL_ENABLE_FLOOR_LIMIT													0x04U

//DRL Result
#define AEX_DRL_RESULT_NONE															0x00U
#define AEX_DRL_RESULT_EXCEED_TRANSACTION_LIMIT										0x01U
#define AEX_DRL_RESULT_REQUIRE_CVM													0x02U
#define AEX_DRL_RESULT_EXCEED_FLOOR_LIMIT											0x04U

//Unpredictable Number Range
#define AEX_RANGE_UNPREDICTABLE_NUMBER												60

//Deactivation Timer
#define AEX_TIMER_DEACTIVATION														15

//Pseudo Magnetic Stripe Track Length
#define AEX_PMS_LENGTH_TRACK_ONE													62
#define AEX_PMS_LENGTH_TRACK_TWO													39

//Pseudo Magnetic Stripe Track Field Offset
#define AEX_PMS_LENGTH_TRACK_ONE_START_SENTINEL										1
#define AEX_PMS_LENGTH_TRACK_ONE_FORMAT_CODE										1
#define AEX_PMS_LENGTH_TRACK_ONE_PAN												15
#define AEX_PMS_LENGTH_TRACK_ONE_FIELD_SEPARATOR_1									1
#define AEX_PMS_LENGTH_TRACK_ONE_CARDHOLDER_NAME									21
#define AEX_PMS_LENGTH_TRACK_ONE_ATC												5
#define AEX_PMS_LENGTH_TRACK_ONE_FIELD_SEPARATOR_2									1
#define AEX_PMS_LENGTH_TRACK_ONE_APPLICATION_EXPIRATION_DATE						4
#define AEX_PMS_LENGTH_TRACK_ONE_SERVICE_CODE										3
#define AEX_PMS_LENGTH_TRACK_ONE_UNPREDICTABLE_NUMBER								4
#define AEX_PMS_LENGTH_TRACK_ONE_CRYPTOGRAM											5
#define AEX_PMS_LENGTH_TRACK_ONE_END_SENTINEL										1

#define AEX_PMS_LENGTH_TRACK_TWO_START_SENTINEL										1
#define AEX_PMS_LENGTH_TRACK_TWO_PAN												15
#define AEX_PMS_LENGTH_TRACK_TWO_FIELD_SEPARATOR									1
#define AEX_PMS_LENGTH_TRACK_TWO_APPLICATION_EXPIRATION_DATE						4
#define AEX_PMS_LENGTH_TRACK_TWO_SERVICE_CODE										3
#define AEX_PMS_LENGTH_TRACK_TWO_UNPREDICTABLE_NUMBER								4
#define AEX_PMS_LENGTH_TRACK_TWO_CRYPTOGRAM											5
#define AEX_PMS_LENGTH_TRACK_TWO_ATC												5
#define AEX_PMS_LENGTH_TRACK_TWO_END_SENTINEL										1

//Pseudo Magnetic Stripe Track Field Offset
#define AEX_PMS_OFFSET_TRACK_ONE_START_SENTINEL										0
#define AEX_PMS_OFFSET_TRACK_ONE_FORMAT_CODE										(AEX_PMS_OFFSET_TRACK_ONE_START_SENTINEL				+AEX_PMS_LENGTH_TRACK_ONE_START_SENTINEL				)
#define AEX_PMS_OFFSET_TRACK_ONE_PAN												(AEX_PMS_OFFSET_TRACK_ONE_FORMAT_CODE					+AEX_PMS_LENGTH_TRACK_ONE_FORMAT_CODE					)
#define AEX_PMS_OFFSET_TRACK_ONE_FIELD_SEPARATOR_1									(AEX_PMS_OFFSET_TRACK_ONE_PAN							+AEX_PMS_LENGTH_TRACK_ONE_PAN							)
#define AEX_PMS_OFFSET_TRACK_ONE_CARDHOLDER_NAME									(AEX_PMS_OFFSET_TRACK_ONE_FIELD_SEPARATOR_1				+AEX_PMS_LENGTH_TRACK_ONE_FIELD_SEPARATOR_1				)
#define AEX_PMS_OFFSET_TRACK_ONE_ATC												(AEX_PMS_OFFSET_TRACK_ONE_CARDHOLDER_NAME				+AEX_PMS_LENGTH_TRACK_ONE_CARDHOLDER_NAME				)
#define AEX_PMS_OFFSET_TRACK_ONE_FIELD_SEPARATOR_2									(AEX_PMS_OFFSET_TRACK_ONE_ATC							+AEX_PMS_LENGTH_TRACK_ONE_ATC							)
#define AEX_PMS_OFFSET_TRACK_ONE_APPLICATION_EXPIRATION_DATE						(AEX_PMS_OFFSET_TRACK_ONE_FIELD_SEPARATOR_2				+AEX_PMS_LENGTH_TRACK_ONE_FIELD_SEPARATOR_2				)
#define AEX_PMS_OFFSET_TRACK_ONE_SERVICE_CODE										(AEX_PMS_OFFSET_TRACK_ONE_APPLICATION_EXPIRATION_DATE	+AEX_PMS_LENGTH_TRACK_ONE_APPLICATION_EXPIRATION_DATE	)
#define AEX_PMS_OFFSET_TRACK_ONE_UNPREDICTABLE_NUMBER								(AEX_PMS_OFFSET_TRACK_ONE_SERVICE_CODE					+AEX_PMS_LENGTH_TRACK_ONE_SERVICE_CODE					)
#define AEX_PMS_OFFSET_TRACK_ONE_CRYPTOGRAM											(AEX_PMS_OFFSET_TRACK_ONE_UNPREDICTABLE_NUMBER			+AEX_PMS_LENGTH_TRACK_ONE_UNPREDICTABLE_NUMBER			)
#define AEX_PMS_OFFSET_TRACK_ONE_END_SENTINEL										(AEX_PMS_OFFSET_TRACK_ONE_CRYPTOGRAM					+AEX_PMS_LENGTH_TRACK_ONE_CRYPTOGRAM					)

#define AEX_PMS_OFFSET_TRACK_TWO_START_SENTINEL										0
#define AEX_PMS_OFFSET_TRACK_TWO_PAN												(AEX_PMS_OFFSET_TRACK_TWO_START_SENTINEL				+AEX_PMS_LENGTH_TRACK_TWO_START_SENTINEL				)
#define AEX_PMS_OFFSET_TRACK_TWO_FIELD_SEPARATOR									(AEX_PMS_OFFSET_TRACK_TWO_PAN							+AEX_PMS_LENGTH_TRACK_TWO_PAN							)
#define AEX_PMS_OFFSET_TRACK_TWO_APPLICATION_EXPIRATION_DATE						(AEX_PMS_OFFSET_TRACK_TWO_FIELD_SEPARATOR				+AEX_PMS_LENGTH_TRACK_TWO_FIELD_SEPARATOR				)
#define AEX_PMS_OFFSET_TRACK_TWO_SERVICE_CODE										(AEX_PMS_OFFSET_TRACK_TWO_APPLICATION_EXPIRATION_DATE	+AEX_PMS_LENGTH_TRACK_TWO_APPLICATION_EXPIRATION_DATE	)
#define AEX_PMS_OFFSET_TRACK_TWO_UNPREDICTABLE_NUMBER								(AEX_PMS_OFFSET_TRACK_TWO_SERVICE_CODE					+AEX_PMS_LENGTH_TRACK_TWO_SERVICE_CODE					)
#define AEX_PMS_OFFSET_TRACK_TWO_CRYPTOGRAM											(AEX_PMS_OFFSET_TRACK_TWO_UNPREDICTABLE_NUMBER			+AEX_PMS_LENGTH_TRACK_TWO_UNPREDICTABLE_NUMBER			)
#define AEX_PMS_OFFSET_TRACK_TWO_ATC												(AEX_PMS_OFFSET_TRACK_TWO_CRYPTOGRAM					+AEX_PMS_LENGTH_TRACK_TWO_CRYPTOGRAM					)
#define AEX_PMS_OFFSET_TRACK_TWO_END_SENTINEL										(AEX_PMS_OFFSET_TRACK_TWO_ATC							+AEX_PMS_LENGTH_TRACK_TWO_ATC							)

//CVM Result
#define AEX_CVM_RESULT_CVM_PROCESSING												0x00U
#define AEX_CVM_RESULT_CVM_LIST_PROCESSING											0x01U
#define AEX_CVM_RESULT_ONLINE_PIN													0x02U
#define AEX_CVM_RESULT_MOBILE_CVM													0x03U
#define AEX_CVM_RESULT_SIGNATURE													0x04U
#define AEX_CVM_RESULT_NO_CVM_REQUIRED												0x05U
#define AEX_CVM_RESULT_CARDHOLDER_VERIFICATION_UNABLE_TO_COMPLETE					0x06U
#define AEX_CVM_RESULT_CVM_LIMIT_NOT_EXCEEDED										0x07U
#define AEX_CVM_RESULT_CVM_LIMIT_NOT_EXCEEDED_MOBILE								0x08U
#define AEX_CVM_RESULT_CVM_LIMIT_NOT_EXCEEDED_CARD									0x09U
#define AEX_CVM_RESULT_GO_TO_TERMINAL_RISK_MANAGEMENT								0x0AU
#define AEX_CVM_RESULT_TRY_AGAIN													0x0BU
#define AEX_CVM_RESULT_TRY_ANOTHER_INTERFACE										0x0CU
#define AEX_CVM_RESULT_ERROR														0xFFU

//AC Type
#define AEX_ACT_ARQC																0x80U
#define AEX_ACT_TC																	0x40U
#define AEX_ACT_AAC																	0x00U

//Reference Control Parameter
#define AEX_RCP_CDA_SIGNATURE_REQUESTED												0x10U

//82 - Application Interchange Profile
#define AEX_AIP_SDA_SUPPORTED														0x40U
#define AEX_AIP_CARDHOLDER_VERIFICATION_SUPPORTED									0x10U
#define AEX_AIP_TERMINAL_RISK_MANAGEMENT_IS_TO_BE_PERFORMED							0x08U
#define AEX_AIP_CDA_SUPPORTED														0x01U
#define AEX_AIP_EMV_AND_MAGSTRIPE_MODES_SUPPORTED									0x80U
#define AEX_AIP_EXPRESSPAY_MOBILE_SUPPORTED											0x40U
#define AEX_AIP_EXPRESSPAY_MOBILE_HCE_IS_SUPPORTED									0x20U

//95 - Terminal Verification Results
#define AEX_TVR_OFFLINE_DATA_AUTHENTICATION_WAS_NOT_PERFORMED						0x80U
#define AEX_TVR_SDA_FAILED															0x40U
#define AEX_TVR_ICC_DATA_MISSING													0x20U
#define AEX_TVR_CARD_APPEARS_ON_TERMINAL_EXCEPTION_FILE								0x10U
#define AEX_TVR_DDA_FAILED															0x08U
#define AEX_TVR_CDA_FAILED															0x04U
#define AEX_TVR_SDA_SELECTED														0x02U
#define AEX_TVR_ICC_AND_TERMINAL_HAVE_DIFFERENT_APPLICATION_VERSIONS				0x80U
#define AEX_TVR_EXPIRED_APPLICATION													0x40U
#define AEX_TVR_APPLICATION_NOT_YET_EFFECTIVE										0x20U
#define AEX_TVR_REQUESTED_SERVICE_NOT_ALLOWED_FOR_CARD_PRODUCT						0x10U
#define AEX_TVR_NEW_CARD															0x08U
#define AEX_TVR_CARDHOLDER_VERIFICATION_WAS_NOT_SUCCESSFUL							0x80U
#define AEX_TVR_UNRECOGNISED_CVM													0x40U
#define AEX_TVR_PIN_TRY_LIMIT_EXCEEDED												0x20U
#define AEX_TVR_PIN_ENTRY_REQUIRED_AND_PIN_PAD_NOT_PRESENT_OR_NOT_WORKING			0x10U
#define AEX_TVR_PIN_ENTRY_REQUIRED_PIN_PAD_PRESENT_BUT_PIN_WAS_NOT_ENTERED			0x08U
#define AEX_TVR_ONLINE_PIN_ENTERED													0x04U
#define AEX_TVR_TRANSACTION_EXCEEDS_FLOOR_LIMIT										0x80U
#define AEX_TVR_LOWER_CONSECUTIVE_OFFLINE_LIMIT_EXCEEDED							0x40U
#define AEX_TVR_UPPER_CONSECUTIVE_OFFLINE_LIMIT_EXCEEDED							0x20U
#define AEX_TVR_TRANSACTION_SELECTED_RANDOMLY_FOR_ONLINE_PROCESSING					0x10U
#define AEX_TVR_MERCHANT_FORCED_TRANSACTION_ONLINE									0x08U
#define AEX_TVR_DEFAULT_TDOL_USED													0x80U
#define AEX_TVR_ISSUER_AUTHENTICATION_FAILED										0x40U
#define AEX_TVR_SCRIPT_PROCESSING_FAILED_BEFORE_FINAL_GENERATEAC					0x20U
#define AEX_TVR_SCRIPT_PROCESSING_FAILED_AFTER_FINAL_GENERATEAC						0x10U

//9C - Transaction Type
#define AEX_TXT_PURCHASE															0x00U
#define AEX_TXT_CASH																0x01U
#define AEX_TXT_CASHBACK															0x09U

//9F07 - Application Usage Control
#define AEX_AUC_VALID_FOR_DOMESTIC_CASH_TRANSACTIONS								0x80U
#define AEX_AUC_VALID_FOR_INTERNATIONAL_CASH_TRANSACTIONS							0x40U
#define AEX_AUC_VALID_FOR_DOMESTIC_GOODS											0x20U
#define AEX_AUC_VALID_FOR_INTERNATIONAL_GOODS										0x10U
#define AEX_AUC_VALID_FOR_DOMESTIC_SERVICES											0x08U
#define AEX_AUC_VALID_FOR_INTERNATIONAL_SERVICES									0x04U
#define AEX_AUC_VALID_AT_ATMS														0x02U
#define AEX_AUC_VALID_AT_TERMINALS_OTHER_THAN_ATMS									0x01U
#define AEX_AUC_DOMESTIC_CASHBACK_ALLOWED											0x80U
#define AEX_AUC_INTERNATIONAL_CASHBACK_ALLOWED										0x40U

//9F33 - Terminal Capabilities
#define AEX_TRC_MANUAL_KEY_ENTRY													0x80U
#define AEX_TRC_MAGNETIC_STRIPE														0x40U
#define AEX_TRC_IC_WITH_CONTACTS													0x20U
#define AEX_TRC_PLAIN_TEXT_PIN_FOR_ICC_VERIFICATION									0x80U
#define AEX_TRC_ENCIPHERED_PIN_FOR_ONLINE_VERIFICATION								0x40U
#define AEX_TRC_SIGNATURE_PAPER														0x20U
#define AEX_TRC_ENCIPHERED_PIN_FOR_OFFLINE_VERIFICATION								0x10U
#define AEX_TRC_NO_CVM_REQUIRED														0x08U
#define AEX_TRC_SDA																	0x80U
#define AEX_TRC_DDA																	0x40U
#define AEX_TRC_CARD_CAPTURE														0x20U
#define AEX_TRC_CDA																	0x08U

//9F6D - Contactless Reader Capabilities
#define AEX_CRC_EMV_AND_MAGSTRIPE													0x80U
#define AEX_CRC_CVM_REQUIRED														0x08U

//9F6E - Enhanced Contactless Reader Capabilities
#define AEX_ECRC_AEIPS_CONTACT_MODE_SUPPORTED										0x80U
#define AEX_ECRC_Expresspay_MAGSTRIPE_MODE_SUPPORTED								0x40U
#define AEX_ECRC_Expresspay_EMV_PARTIAL_ONLINE_MODE_SUPPORTED						0x10U
#define AEX_ECRC_Expresspay_MOBILE_SUPPORTED										0x08U
#define AEX_ECRC_MOBILE_CVM_SUPPORTED												0x80U
#define AEX_ECRC_ONLINE_PIN_SUPPORTED												0x40U
#define AEX_ECRC_SIGNATURE															0x20U
#define AEX_ECRC_PLAINTEXT_OFFLINE_PIN												0x10U
#define AEX_ECRC_TERMINAL_IS_OFFLINE_ONLY											0x80U
#define AEX_ECRC_CVM_REQUIRED														0x40U

//9F70 - Card Interface and Payment Capabilities
#define AEX_CIPC_KEYED_DATA_ENTRY_SUPPORTED											0x80U
#define AEX_CIPC_PHYSICAL_MAGNETIC_STRIPE_SUPPORTED									0x40U
#define AEX_CIPC_CONTACT_EMV_INTERFACE_SUPPORTED									0x20U
#define AEX_CIPC_CONTACTLESS_EMV_INTERFACE_SUPPORTED								0x10U
#define AEX_CIPC_MOBILE_INTERFACE_SUPPORTED											0x08U
#define AEX_CIPC_DELAYED_AUTHORIZATION_USAGE_INFORMATION_PRESENT					0x80U
#define AEX_CIPC_VALID_AT_DOMESTIC_TERMINALS_PERFORMING_DELAYED_AUTHORIZATION		0x40U
#define AEX_CIPC_VALID_AT_INTERNATIONAL_TERMINALS_PERFORMING_DELAYED_AUTHORIZATION	0x20U

//9F71 - Mobile CVM Results
#define AEX_MCR_MOBILE_CVM_PERFORMED												0x01U
#define AEX_MCR_NO_CVM_PERFORMED													0x3FU
#define AEX_MCR_MOBILE_CVM_NOT_REQUIRED												0x00U
#define AEX_MCR_TERMINAL_REQUIRED_CVM												0x03U
#define AEX_MCR_UNKNOWN																0x00U
#define AEX_MCR_MOBILE_CVM_FAILED													0x01U
#define AEX_MCR_MOBILE_CVM_SUCCESSFUL												0x02U
#define AEX_MCR_MOBILE_CVM_BLOCKED													0x03U

//CVM Method
#define AEX_CVM_METHOD_FAIL_CVM_PROCESSING															0x00U
#define AEX_CVM_METHOD_PLAINTEXT_PIN_VERIFICATION_PERFORMED_BY_ICC									0x01U
#define AEX_CVM_METHOD_ENCIPHERED_PIN_VERIFIED_ONLINE												0x02U
#define AEX_CVM_METHOD_PLAINTEXT_PIN_VERIFICATION_PERFORMED_BY_ICC_AND_SIGNATURE_PAPER				0x03U
#define AEX_CVM_METHOD_ENCIPHERED_PIN_VERIFICATION_PERFORMED_BY_ICC									0x04U
#define AEX_CVM_METHOD_ENCIPHERED_PIN_VERIFICATION_PERFORMED_BY_ICC_AND_SIGNATURE_PAPER				0x05U
#define AEX_CVM_METHOD_SIGNATURE_PAPER																0x1EU
#define AEX_CVM_METHOD_NO_CVM_REQUIRED																0x1FU

//CVM Condition
#define AEX_CVM_CONDITION_ALWAYS																	0x00U
#define AEX_CVM_CONDITION_IF_UNATTENDED_CASH														0x01U
#define AEX_CVM_CONDITION_IF_NOT_UNATTENDED_CASH_AND_NOT_MANUAL_CASH_AND_NOT_PURCHASE_WITH_CASHBACK	0x02U
#define AEX_CVM_CONDITION_IF_TERMINAL_SUPPORTS_THE_CVM												0x03U
#define AEX_CVM_CONDITION_IF_MANUAL_CASH															0x04U
#define AEX_CVM_CONDITION_IF_PURCHASE_WITH_CASHBACK													0x05U
#define AEX_CVM_CONDITION_IF_TRANSACTION_IS_IN_THE_APPLICATION_CURRENCY_AND_IS_UNDER_X_VALUE		0x06U
#define AEX_CVM_CONDITION_IF_TRANSACTION_IS_IN_THE_APPLICATION_CURRENCY_AND_IS_OVER_X_VALUE			0x07U
#define AEX_CVM_CONDITION_IF_TRANSACTION_IS_IN_THE_APPLICATION_CURRENCY_AND_IS_UNDER_Y_VALUE		0x08U
#define AEX_CVM_CONDITION_IF_TRANSACTION_IS_IN_THE_APPLICATION_CURRENCY_AND_IS_OVER_Y_VALUE			0x09U

