
/**
* @brief Image capture task
*
* This task will get called from a periodic trigger. After checking if the
* camera is ready to be used, it will take the photo storing it in one of the
* local SRAM memory locations. Uses UART that operates at a 57600bsp, with 8 data bits, 1 stop bit
* and no parity checking 
*
* @header software-and-command/main-application
* @param superresolution selections will be used to determine if super resoltuion will be conducted or not
* @pre None
* @post Store one image into the Cubesense memory
* @return None
*/

queue.h
//Sending Telecommands: 0x1F,0x7F,.....,0x1F,0xFF



void captureTask(void *superresolution ){
    long captureSRAM10 = 0x1F7F210001FFF; // Telecommand 21, camera 1, SRAM 1, Top Half
    long captureSRAM11 = 0x1F7F210011FFF; // Telecommand 21, camera 1, SRAM 1, bottom Half
    //SRAM 2 Top half is reserved for ADCS
    long captureSRAM21 = 0x1F7F210111FFF; // Telecommand 21, camera 1, SRAM 2, Bottom Half
    long resultSRAM10 = 0x1F7F210F0FF; // telemetry, camera,  
    int resultsCubeSense;

    //Binary operators, |, Bitwise, <<1, 1>> 

    struct AMessage{
        char RxMessage
    }

    while(1){
        if (superresolution == 1){
            //send capture image telecommand 21 to camera
            //Checks if image was successfully captured
            //Check of camera is ready 
            //Send capture image telecommand 3 times
                //sort image into SRAM

        }
        else{
            //Send Capture command 
            xQueueSend(QueueHandle_t xQueueCubeSense, (void*)&captureSRAM10, (TickType_t) 0);
            //Send request for telemetry for successful image capture
            xQueueSend(QueueHandle_t xQueueCubeSense, (void*)&resultSRAM10, (TickType_t) 15);
            //Saves successful image capture telemetry to 
            if( xQueueCubeSense != NULL){
                if(xQueueRecieve(QueueHandle_t xQueueCubeSense, &(RxMessage), (TickType_t) 10) == pdPASSS){
                    //RxMessage now holds the most recent item in the queue 
                }
                   
            }

            //will have to loop for each case either way
            
        }
    }
}

//https://www.freertos.org/a00118.html