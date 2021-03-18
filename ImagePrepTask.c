/**
* @brief Image Preparation Task
*
*  The image preparation task is responsible for downloading the image taken from the Capture task, 
*  filtering unwanted photos, and preparing for downlinks.
*
* @header software-and-command/main-application
* @param size determined whether or not the photo to deal with is full or thumbnail
* @param ImageID is the unique ID of the photo 
* @pre None
* @post For full image downlink it will raise SRAM slot flag
* @return None
*/

//TODO: change type to char

void imagePrepTask(void *size, void *ImageID){
    long imgDownloadSRAM1TopThumb = 0x1F7F400041FFF;
    long imgDownloadSRAM1BotThumb = 0x1F7F400141FFF;
    long imgDownloadSRAM2BotThumb = 0x1F7F401141FFF;
    long imgDownloadSRAM1TopFull = 0x1F7F400001FFF;
    long imgDownloadSRAM1BotFull = 0x1F7F400101FFF;
    long imgDownloadSRAM2BotFull = 0x1F7F401101FFF;

    //Case 1: Thumbnail
        //send telecommand
        //check success of telecommand (if failed send again)
        //Timed feature ^

        //call quality check function
            
        //Compress image with library

        //send to downlink people

    //Case 2: Full photo
        //send telecommand
        //check success of telecommand (if failed send again)
        //Timed feature ^

        //call quality check function
            
        //Compress image with library

        //send to downlink people
}



//Struct for image ID and image