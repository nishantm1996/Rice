/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

uint8 sendNotifications; /* flag to check notification enabled/disabled */
CYBLE_API_RESULT_T	apiResult; /* variable to store BLE component API return */
int16 result;

/* update LED handle */
void updateLed()
{
    CYBLE_GATTS_HANDLE_VALUE_NTF_T 	tempHandle;
   
    uint8 LED_State = !LED_Read();
    
    if(CyBle_GetState() != CYBLE_STATE_CONNECTED)
        return;
    
    tempHandle.attrHandle = CYBLE_ADC_READ_LED_CHAR_HANDLE;
  	tempHandle.value.val = (uint8 *) &LED_State;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
}

/* update ADC handle and send notifications depending on flag */
void sendADCNotification(void)
{
	if(CyBle_GetState() != CYBLE_STATE_CONNECTED)
        return;
    
    CYBLE_GATTS_HANDLE_VALUE_NTF_T		ADCHandle; /* stores  notification data parameters */
	    
    ADCHandle.attrHandle = CYBLE_ADC_READ_ADC_READ_DATA_CHAR_HANDLE;

    ADCHandle.value.val = (uint8 *)&result;
    
	ADCHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&ADCHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED );
    if (sendNotifications)
        CyBle_GattsNotification(cyBle_connHandle,&ADCHandle);        
}

/* call back function for BLE */
void AppCallBack(uint32 event, void *eventParam)
{
	CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;

	switch (event)
	{
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
        sendNotifications = 0;
        CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);            
        break;
	
        case CYBLE_EVT_GATT_CONNECT_IND:
        /* call LED and ADC update functions */
        updateLed();    
        sendADCNotification();  
		break;

        case CYBLE_EVT_GATTS_WRITE_REQ:
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
			
            /* request write the LED value */
            if(wrReqParam->handleValPair.attrHandle == CYBLE_ADC_READ_LED_CHAR_HANDLE)
            {
                /* only update the value and write the response if the requested write is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_LOCALLY_INITIATED))
                {
                    LED_Write(!wrReqParam->handleValPair.value.val[0]);
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                }
            }
                       
            /* request to update the CapSense notification */
            if(wrReqParam->handleValPair.attrHandle == CYBLE_ADC_READ_ADC_READ_DATA_ADC_READCCCD_DESC_HANDLE) 
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                sendNotifications = wrReqParam->handleValPair.value.val[0] & 0x01;
                CyBle_GattsWriteRsp(cyBle_connHandle);
            }		
		break;
		

	default:
		break;
	}
}

int main(void)
{
    /* startup code */
    op1_Start();
    op2_Start();
    op3_Start();
    op4_Start();
    ADC_Start();
    CyGlobalIntEnable; /* enable global interrupts */
    ADC_StartConvert();           
    CyBle_Start(AppCallBack);
    for(;;)
    {
        /* application code */
        CyBle_ProcessEvents();
		 
	    if (ADC_IsEndConversion(ADC_RETURN_STATUS))
        {
            result = ADC_GetResult16(0); /* get 16-bit ADC result from channel 0 */            
            sendADCNotification(); /* send notifications */
        }
        CyBle_ProcessEvents();
    }
}
/* [] END OF FILE */
