
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

//An Error that may come up, Telecommands ID were assumed to be in hex already (i.e. 21 is 0x21)

queue.h;
int SRAM1Top = 0;       //flag to check if SRAM slot is open
int SRAM1Bottom = 0;    //flag to check if SRAM slot is open
int SRAM2Bottom = 0;    //flag to check if SRAM slot is open


void captureLocation(void *SRAMSlot){
    long TLcapatureStatus = 0x1F7F941FFF; // telemetry 20, sensor 1,  
    int resultsCubeSense;

    struct AMessage{
        char RxMessage
    }

    //Send Capture command 
    xQueueSend(QueueHandle_t xQueueCubeSense, void *SRAMSlot, (TickType_t) 0);
    //Send request for telemetry for successful image capture
    xQueueSend(QueueHandle_t xQueueCubeSense, (void*)&TLcapatureStatus, (TickType_t) 15);
    //Saves successful image capture telemetry to 
    if( xQueueCubeSense != NULL){
        xQueueRecieve(QueueHandle_t xQueueCubeSense, void *(RxMessage), (TickType_t) 10); 
        resultsCubeSense = RxMessage & 0x000000001; //mask with byte 4 for status
    }
    int breakFlag;
    while(resultsCubeSense != 0x2 | resultsCubeSense != 0x3 | breakFlag = 0){ //does not equal successful capture 
            breakFlag = 0;
        if (resultsCubeSense == 0x4){
            breakFlag = 1;
        }
    }
}

void captureTask(void *superresolution ){
    long TCcaptureSRAM10 = 0x1F7F150001FFF; // Telecommand 21, camera 1, SRAM 1, Top Half
    long TCcaptureSRAM11 = 0x1F7F151001FFF; // Telecommand 21, camera 1, SRAM 1, bottom Half
    // long captureSRAM11 = 0x1F7F210101FFF; // Telecommand 21, camera 1, SRAM 2, Top Half Reserved for ADCS
    long TCcaptureSRAM21 = 0x1F7F151101FFF; // Telecommand 21, camera 1, SRAM 2, Bottom Half
    long TLcapatureStatus = 0x1F7F941FFF; // telemetry 20, sensor 1,  
    

    struct AMessage{
        char RxMessage
    }

    while(1){
        if (superresolution == 1){
            captureLocation(long &captureSRAM10);
            captureLocation(long *captureSRAM11);
            captureLocation(long *captureSRAM21);
        }
        else{

            if(SRAM1Top){
                captureLocation(long *captureSRAM10);
            }
            else if(SRAM1Bottom){
                captureLocation(long *captureSRAM11);
            }
            else if(SRAM2Bottom){
                captureLocation(long *captureSRAM21);
            }
            else{
                break;
            }            
        }
    }
}

//-------------Useful Notes--------------


//https://www.freertos.org/a00118.html


// if current way of doing regular loop doesnt work try this if statement way
// if (~flag1 || ~flag2 || ~flag3){
//}