#include "ColumnListView.h"
#include "ColumnTypes.h"
#include "MameIO.h"
#include "Utilities.h"

#include <cstdio>
#include <unistd.h>

#include <File.h>
#include <Entry.h>
#include <List.h>
#include <String.h>
#include <TextView.h>
#include <Window.h>

bool TMameInterface::sGenerated = false;
BPath TMameInterface::sMamePath;
BPath TMameInterface::sDestPath;

const char *kRomFile = "gamelist";

Rom::Rom()
{
	Clear();
}


void
Rom::Clear()
{
	sprintf(romName, "Unknown");
	sprintf(description, "Unknown");
	sprintf(cloneOf, "-");
	sprintf(year, "-");
	sprintf(manufacturer, "-");	
}


TMameInterface::TMameInterface(const char *executablePath, const char *destPath)
{
	if (executablePath != NULL)
		sMamePath.SetTo(executablePath);
	if (destPath != NULL) {
		sDestPath.SetTo(destPath);
		sDestPath.Append(kRomFile);
	}	
	
	if (!sGenerated && InitCheck() == B_OK) {
		sGenerated = true;
		BString cmd;
		cmd << sMamePath.Path() << " -listinfo > " << sDestPath.Path();
 		system(cmd.String());
	}
	
	fRomFile = fopen(sDestPath.Path(), "r");
}


TMameInterface::~TMameInterface()
{
	if (fRomFile)
		fclose(fRomFile);
}


status_t
TMameInterface::InitCheck()
{
	if (BEntry(sMamePath.Path()).Exists())
		return B_OK;
	return B_FILE_NOT_FOUND;
}


void
TMameInterface::Rewind()
{
	if (fRomFile)
		fseek(fRomFile, SEEK_SET, 0);
}


status_t
TMameInterface::GetNextRom(Rom *rom)
{
	char *p = NULL;
	char *value = NULL;
	char *keyword = NULL;        
	char line[500];
	int32 i = 0;
	int32 tmpCounter = 0;
	char *chip[16];
	char **tmpArray;
	
	for (int32 x = 0; x < 16; x++)
    	chip[x] = NULL;
    	
	while (fgets(line, 500, fRomFile)) {
    	line[strlen(line) - 1] = 0;
        if (!strcmp(line, "game (")) {
        	while (fgets(line, 500, fRomFile)) {
            	if (line[0] == ')')
                	return B_OK;
                p = line + 1;
                keyword = p;
                i = 0;

				while (*p && (*p++ != ' '))
                	i++;
                keyword[i] = 0;

                if (p[0] == '\"')
                	*p = *p++;
                value = p;
                
                i = 0;
                
				while (*p && (*p++ != '\n'))
                  	i++;

				if (value[i - 1] == '\"')
                  	i--;
                value[i] = '\0';

                if (!strcmp(keyword, "name")) {
                  	snprintf(rom->romName, MAX_ROMNAME, value);
                }  
                if (!strcmp(keyword, "description")) {
                   	snprintf(rom->description, MAX_GAMENAME, value);
                }    
                if (!strcmp(keyword, "year")) {
                   	snprintf(rom->year, MAX_YEAR, value);
                }
                if (!strcmp(keyword, "cloneof")) {
                    snprintf(rom->cloneOf, MAX_ROMNAME, value);	
                }                 
                if (!strcmp(keyword, "manufacturer")) {
                    snprintf(rom->manufacturer, MAX_MANU, value);
                }
                if (!strcmp(keyword, "chip")) {                  	
                   	// Put it in the next free chip[] slot
                  	tmpCounter = 0;
                  	while ((chip[tmpCounter] != NULL)
                   			&& tmpCounter < 16)
                   		tmpCounter++;
               	 	if ((chip[tmpCounter] == NULL))
                   		chip[tmpCounter] = strdup(value);
                }
                tmpCounter = 0;
        		while ((chip[tmpCounter] != NULL)
        			&& (tmpCounter < 16)) {
       				tmpArray = strsplit(chip[tmpCounter], " ", MAX_CPU);
       			
            		if (!strcmp(tmpArray[3], "name")) {
            			if (!strcmp(tmpArray[2], "cpu")) {
                			sprintf(rom->cpu1, "%s", tmpArray[4]);
                    		sprintf(rom->cpu2, "%s", tmpArray[4]);
                    		sprintf(rom->cpu3, "%s", tmpArray[4]);
                    		sprintf(rom->cpu4, "%s", tmpArray[4]);
                		}
                		if (!strcmp(tmpArray[2], "audio")) {
                			sprintf(rom->sound1, "%s", tmpArray[4]);
                    		sprintf(rom->sound2, "%s", tmpArray[4]);
                    		sprintf(rom->sound3, "%s", tmpArray[4]);
                    		sprintf(rom->sound4, "%s", tmpArray[4]);
                		}
           			}
            		strfreev(tmpArray);
            		tmpCounter++;
     			}
			
        		tmpCounter = 0;
        		while ((chip[tmpCounter] != NULL)) {
        			delete[] chip[tmpCounter];
            		chip[tmpCounter] = NULL;
            		tmpCounter++;
        		}        		
			}
		}
	}
	
	return B_ERROR;
}


#if 0
status_t
TMameInterface::GetRomList(const char *destFile,
//		bool generate, BList *romList)
		bool generate, BColumnListView *romList)
{
	if (generate) {
		printf("SettingsPath: %s\n", destFile);
		BString cmd;
		cmd << sMamePath.Path();
		if (!BEntry(cmd.String()).Exists())
			return B_ERROR;
		
		cmd << " -listinfo > " << destFile;
 		system(cmd.String());
	}
		
    char *p = NULL;
	char *value = NULL;
	char *keyword = NULL;
	char *verifyName = NULL;              
	char line[500];
	int32 i = 0;	
	
	FILE *mameInfo = fopen(destFile, "r");
	if (mameInfo == NULL)
		return B_ERROR;
	
	char * chip[16];
    char **tmpArray;
    for (int32 x = 0; x < 16; x++)
    	chip[x] = NULL;   	 	
    int32 tmpCounter = 0;
    
    BWindow *window = romList->Window();
    while (fgets(line, 500, mameInfo)) {
    	line[strlen(line) - 1] = 0;
        if (!strcmp(line, "game (")) {
        	BRow *newItem = new BRow();
        	BStringField *description = new BStringField("");
        	BStringField *romName = new BStringField("");
        	BStringField *year = new BStringField("");
        	BStringField *cloneOf = new BStringField("");
        	BStringField *manufacturer = new BStringField("");
        	
        	//GameItem *newItem = new GameItem();
        	char tmp[500];
        	      	
        	snprintf(tmp, MAX_ROMNAME, "Unknown");
            description->SetString(tmp);
            romName->SetString(tmp);
            year->SetString(tmp);                	
	        snprintf(tmp, MAX_ROMNAME, "-");
            cloneOf->SetString(tmp);                   
            snprintf(tmp, MAX_MANU, "Unknown");
            manufacturer->SetString(tmp);
            /*snprintf(romOf, MAX_ROMNAME, "-");
            */
            while (fgets(line, 500, mameInfo)) {
            	if (line[0] == ')')
                	break;
                p = line + 1;
                keyword = p;
                i = 0;

				while (*p && (*p++ != ' '))
                	i++;
                keyword[i] = 0;

                if (p[0] == '\"')
                	*p = *p++;
                value = p;
                
                i = 0;
                
				while (*p && (*p++ != '\n'))
                  	i++;

				if (value[i - 1] == '\"')
                  	i--;
                value[i] = '\0';

                if (!strcmp(keyword, "name")) {
                  	snprintf(tmp, MAX_ROMNAME, value);
                  	//newItem->SetColumnContent(1, tmp, false);
                	romName->SetString(tmp);
                }
                    
                if (!strcmp(keyword, "description")) {
                   	snprintf(tmp, MAX_GAMENAME, value);
                	//newItem->SetColumnContent(0, tmp, false);
                	description->SetString(tmp);
                }    
                if (!strcmp(keyword, "year")) {
                   	snprintf(tmp, MAX_YEAR, value);
                	year->SetString(tmp);
                }
                if (!strcmp(keyword, "cloneof")) {
                    snprintf(tmp, MAX_ROMNAME, value);
                	cloneOf->SetString(tmp);
                }
                  
                if (!strcmp(keyword, "manufacturer")) {
                    snprintf(tmp, MAX_MANU, value);
                	manufacturer->SetString(tmp);
                }
                
                //for (int32 x = 0; x < 16; x++)
    			//	chip[x] = NULL;   
                
                 /*                 
                if (!strcmp(keyword, "romof"))
                    snprintf(romOf, MAX_ROMNAME, value);
                */                
                if (!strcmp(keyword, "chip")) {                  	
                   	// Put it in the next free chip[] slot
                  	tmpCounter = 0;
                  	while ((chip[tmpCounter] != NULL)
                   			&& tmpCounter < 16)
                   		tmpCounter++;
               	 	if ((chip[tmpCounter] == NULL))
                   		chip[tmpCounter] = strdup(value);
                }
                /*if (!strcmp(keyword, "video"))
                  	snprintf(newItem->video, 200, value);
                if (!strcmp(keyword, "input"))
                    snprintf(newItem->input, 200, value);
                if (!strcmp(keyword, "driver"))
                    snprintf(newItem->driverStatus, 200, value);
*/
			}
			/*while (TRUE) {
				if (!fgets(line, 500, mameVerify))
               		break;
            	if (strstr(line, "romset")) {
               		verifyName = line + 7;
                	p = verifyName;
                	i = 0;

                	while (*p && (*p++ != ' '))
	                	i++;

 	               	verifyName[i] = 0;
    	            i++;

                    value = line + 7 + i;
                    i++;

                    while (*p && (*p++ != '\n'))
	                    i++;

                    value[i] = '\0';
                    snprintf(status, 200, value);
                    break;
            	}
            }*/
            
        	
	        // Setting some default values
    	    /*snprintf(newItem->cpu1, MAX_ROMNAME, "-");
        	snprintf(newItem->cpu2, MAX_ROMNAME, "-");
	        snprintf(newItem->cpu3, MAX_ROMNAME, "-");
    	    snprintf(newItem->cpu4, MAX_ROMNAME, "-");
        	snprintf(newItem->sound1, MAX_ROMNAME, "-");
       		snprintf(newItem->sound2, MAX_ROMNAME, "-");
        	snprintf(newItem->sound3, MAX_ROMNAME, "-");
        	snprintf(newItem->sound4, MAX_ROMNAME, "-");
        	snprintf(newItem->control, MAX_CONTROL,"-");
        	newItem->numButtons = 0;
        	newItem->numPlayers = 0;
			*/
        	tmpCounter = 0;
        	while ((chip[tmpCounter] != NULL)
        			&& (tmpCounter < 16)) {
       			tmpArray = strsplit(chip[tmpCounter], " ", MAX_CPU);
       			
            	/*if (!strcmp(tmpArray[3], "name")) {
            		if (!strcmp(tmpArray[2], "cpu")) {
                		if (!strcmp(newItem->cpu1, "-"))
                    		sprintf(newItem->cpu1, "%s", tmpArray[4]);
                    	else if (!strcmp(newItem->cpu2, "-"))
                       		sprintf(newItem->cpu2, "%s", tmpArray[4]);
                    	else if (!strcmp(newItem->cpu3, "-"))
                        	sprintf(newItem->cpu3, "%s", tmpArray[4]);
                    	else if (!strcmp(newItem->cpu4, "-"))
                        	sprintf(newItem->cpu4, "%s", tmpArray[4]);
                	}
                	if (!strcmp(tmpArray[2], "audio")) {
                		if (!strcmp(newItem->sound1, "-"))
                    		sprintf(newItem->sound1, "%s", tmpArray[4]);
                    	else if (!strcmp(newItem->sound2, "-"))
                        	sprintf(newItem->sound2, "%s", tmpArray[4]);
                    	else if (!strcmp(newItem->sound3,"-"))
                        	sprintf(newItem->sound3, "%s", tmpArray[4]);
                    	else if (!strcmp(newItem->sound4, "-"))
                        	sprintf(newItem->sound4, "%s", tmpArray[4]);
                	}
           		}*/
            	strfreev(tmpArray);
            	tmpCounter++;
     		}
			
        	tmpCounter = 0;
        	while ((chip[tmpCounter] != NULL)) {
        		delete[] chip[tmpCounter];
            	chip[tmpCounter] = NULL;
            	tmpCounter++;
        	}
        	/*
        	tmpArray = strsplit(video, " ", MAX_CPU);
        	if (!strcmp(tmpArray[2], "vector"))
        		rom->vector = true;
        	else
          		rom->vector = false;
        	strfreev(tmpArray);               
        	*/
        	/*tmpArray = strsplit(input, " ", MAX_CPU);
            tmpCounter = 0;
            while ((tmpArray[tmpCounter + 1] != NULL)) {
            	if (!strcmp(tmpArray[tmpCounter], "players"))
                	rom->num_players = atoi(tmpArray[tmpCounter + 1]);
                if (!strcmp(tmpArray[tmpCounter], "control"))
                    snprintf(rom->control, MAX_CONTROL, tmpArray[tmpCounter + 1]);
                if (!strcmp(tmpArray[tmpCounter], "buttons"))
                    rom->num_buttons = atoi(tmpArray[tmpCounter + 1]);
                tmpCounter++;
            }
            
            strfreev(tmpArray);*/
/*
            tmpArray = strsplit(driverStatus, " ", MAX_CPU);
            if ((tmpArray[2]) && !strcmp(tmpArray[2], "good")) {
            	rom->working = true;
            } else {
            	rom->working = false;
            }
            strfreev(tmpArray);

            if (!strcmp(status, "correct\n"))
            	rom->status = CORRECT;
            else if (!strcmp(status, "incorrect\n"))
                rom->status = INCORRECT;
            else if (!strcmp(status, "not found\n"))
                rom->status = MISSING;
            else
            	rom->status = MAY_WORK;
        	
        	rom->favourite = false;
        	 */
        	//newItem->SetAvailable(true);     	
        	//romList->AddItem(newItem);
        	newItem->SetField(description, 0);
        	newItem->SetField(romName, 1);
        	newItem->SetField(year, 2);
        	newItem->SetField(cloneOf, 3);
        	newItem->SetField(manufacturer, 4);
        	if (window->LockLooper()) {
        		romList->AddRow(newItem);        	        	
        		window->UnlockLooper();
        	}
        }
    }
           
    fclose(mameInfo);
    
    return B_OK;
}
#endif

int32
TMameInterface::CheckSet(const char *setName)
{
	int32 result = B_OK;
	
	BString cmd;
	cmd << sMamePath.Path() << " -vset " << setName;
	printf("%s\n", cmd.String());
 	
 	FILE *file = popen(cmd.String(), "r");
 	if (file != NULL) {
 		char line[500];
 		while (fgets(line, 500, file)) {
 			if (strstr(line, "not found") != NULL ||
 				strstr(line, " bad") != NULL)
 				result = B_ERROR;
 		}
 		pclose(file);
 	}
 	return result;
}


status_t
TMameInterface::MoreCheckSet(const char *setName, BTextView *output)
{
	BString cmd;
	cmd = sMamePath.Path();
 	cmd << " -vset " << setName;
 	printf("%s\n", cmd.String());
 	
 	output->MakeEditable(true);
 	output->SelectAll();
 	output->Clear();
 	
 	FILE *file = popen(cmd.String(), "r");
 	if (file != NULL) {
 		char line[500];
 		while (fgets(line, 500, file)) {
 			output->Insert(line);
		}
 		pclose(file);
 	}
 	output->Insert("----------------------------------------------\n");
 	
 	cmd = sMamePath.Path();
 	cmd << " -listroms " << setName;
 	
	printf("%s\n", cmd.String());
 	file = popen(cmd.String(), "r");
 	if (file != NULL) {
 		char line[500];
 		while (fgets(line, 500, file)) {
 			output->Insert(line);
		}
 		pclose(file);
 	}
 	
 	output->MakeEditable(false);
 	
 	return B_OK;
}


status_t
TMameInterface::Play(const char *options, BString *output)
{
	BString cmd;
	char *tempName = tempnam("/var/tmp", "bemamer_");
	cmd << sMamePath.Path() << " " << options << " > " << tempName;
	
 	int32 result = system(cmd.String());
 	
 	if (result != B_OK) {
 		BFile file(tempName, B_READ_ONLY);
 		int32 size = file.Seek(0, SEEK_END);
 		char *buffer = output->LockBuffer(size + 1);
 		file.Seek(0, SEEK_SET);
 		file.Read(buffer, size);
 		output->UnlockBuffer();
 		
		int32 position = output->FindLast("ERROR") - 1;
		output->Truncate(position);
				
		position = output->FindLast((char)0x0d) + 1;
		output->Remove(0, position);
		
 		BEntry entry(tempName);
 		entry.Remove();
 	}
 	delete tempName;
 	
 	return result;
}
