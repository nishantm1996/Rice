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

uint8 sendNotifications; 	/* Flag to check notification enabled/disabled */
CYBLE_API_RESULT_T	apiResult;  /*  variable to store BLE component API return */
int16 result;
//volatile uint32 windowFlag    = 0u;
//volatile uint8  dataReady     = 0u;
//CY_ISR_PROTO(ADC_ISR_Handler);
void sendADCNotification(void)
{
	if(CyBle_GetState() != CYBLE_STATE_CONNECTED)
        return;
    
    /* stores  notification data parameters */
	CYBLE_GATTS_HANDLE_VALUE_NTF_T		ADCHandle;
    /*
	if (sendNotifications)
	{
		//Package the notification data as part of I2C_read Characteristic
		ADCHandle.attrHandle = CYBLE_ADC_READ_ADC_READ_DATA_CHAR_HANDLE;

		ADCHandle.value.val = result;

		ADCHandle.value.len = 2;

		//Send the I2C_read Characteristic to the client only when notification is enabled 
		do
		{
			apiResult = CyBle_GattsNotification(cyBle_connHandle, &ADCHandle);

			CyBle_ProcessEvents();

		} while ((CYBLE_ERROR_OK != apiResult) && (CYBLE_STATE_CONNECTED == cyBle_state));

	}*/
    
    ADCHandle.attrHandle = CYBLE_ADC_READ_ADC_READ_DATA_CHAR_HANDLE;

    ADCHandle.value.val = (uint8 *)&result;
    
	ADCHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&ADCHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED );
    if (sendNotifications)
        CyBle_GattsNotification(cyBle_connHandle,&ADCHandle);        
}

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
            sendADCNotification();  
		break;

        case CYBLE_EVT_GATTS_WRITE_REQ:
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
			      
                       
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
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    op1_Start();
    op2_Start();
    op3_Start();
    op4_Start();
    ADC_Start();
    //ADC_IRQ_StartEx(ADC_ISR_Handler);
    CyGlobalIntEnable; /* Enable global interrupts. */
    ADC_StartConvert();           
    CyBle_Start(AppCallBack);
    for(;;)
    {
        /* Place your application code here. */
        CyBle_ProcessEvents();
		 
	    if (ADC_IsEndConversion(ADC_RETURN_STATUS))
        {
        //while(dataReady == 0u)
        //{
        //    ; 
        //}    
            result = ADC_GetResult16(0);            
            sendADCNotification();
        }
        CyBle_ProcessEvents();
        //dataReady = 0u;
    }
}
/*
CY_ISR(ADC_ISR_Handler)
{
    uint32 intr_status;

    //Read interrupt status registers 
    intr_status = ADC_SAR_INTR_MASKED_REG;
    //Check for End of Scan interrupt 
    if((intr_status & ADC_EOS_MASK) != 0u)
    {
        //Read range interrupt status and raise the flag 
        windowFlag = ADC_SAR_RANGE_INTR_MASKED_REG;
        //Clear range detect status 
        ADC_SAR_RANGE_INTR_REG = windowFlag;
        dataReady = 1u;
    }
    //Clear handled interrupt 
    ADC_SAR_INTR_REG = intr_status;
}*/
/* [] END OF FILE */
