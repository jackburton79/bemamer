#include "BeMamerWindow.h"
#include "BitmapView.h"
#include "GameAlert.h"
#include "InfoView.h"
#include "MameIO.h"
#include "Messages.h"
#include "MyTabView.h"
#include "Settings.h"
#include "VerifyView.h"

#include <Alert.h>
#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <ColumnListView.h>
#include <ColumnTypes.h>
#include <Entry.h>
#include <File.h>
#include <PopUpMenu.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <StatusBar.h>
#include <String.h>

#include <cstdio>

const char* kLVTabNames[] = {
	"Game Name",
	"Set",
	"Year",
	"Clone of",
	"Manufacturer",
	"Cpu1", 
	"Cpu2",
	"Cpu3",
	"Cpu4",
	"Sound1",
	"Sound2",
	"Sound3",  
	"Sound4",    
	NULL
};

BeMamerWindow::BeMamerWindow()
	:BWindow(Settings().WindowFrame(), "BeMAMEr", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS),
	fFirstShow(true),
	fRomGenerator(B_BAD_THREAD_ID),
	fRomChecker(B_BAD_THREAD_ID)
{

#if !DEBUG
	//Dice::Initialize();
#endif

	fLVIsExpanded = false;
	
	float menuHeight = AddMenuBar();	
	fBackgroundView = new BBox(Bounds(), "Box", B_FOLLOW_ALL_SIDES,
		B_WILL_DRAW|B_FRAME_EVENTS);
	fBackgroundView->MoveBy(0, menuHeight + 1);	
	fBackgroundView->ResizeBy(1, 0);
	
	/* TabViews */
	BRect tabRect(fBackgroundView->Bounds());
	tabRect.top += 15;
	tabRect.bottom -= 40;
	tabRect.right -= 10;
	tabRect.left = tabRect.right - 400;
		
	fTabs = new TTabView(tabRect, "tabs", B_WIDTH_FROM_LABEL,
		B_FOLLOW_TOP_BOTTOM|B_FOLLOW_RIGHT);
		
	BRect boxRect(fBackgroundView->Bounds());
	boxRect.top += 5;
	boxRect.left += 10;
	boxRect.right = tabRect.left - 20;
	boxRect.bottom -= 50;
	
	/* List View */
	BRect listRect(boxRect);
	listRect.top += 10;
	listRect.right -= 20;
	listRect.bottom -= 60;

	fListView = new BColumnListView(listRect, "list", 
				B_FOLLOW_ALL_SIDES, B_WILL_DRAW|B_NAVIGABLE|B_FRAME_EVENTS,
				B_FANCY_BORDER, true);
	fListView->SetSelectionMode(B_SINGLE_SELECTION_LIST);
				
	fListView->AddColumn(new BStringColumn(kLVTabNames[0], 100, 200, 0, B_TRUNCATE_END), 0);
	fListView->AddColumn(new BStringColumn(kLVTabNames[1], 60, 80, 0, B_TRUNCATE_END), 1);
	fListView->AddColumn(new BStringColumn(kLVTabNames[2], 60, 80, 0, B_TRUNCATE_END), 2);
	fListView->AddColumn(new BStringColumn(kLVTabNames[3], 60, 80, 0, B_TRUNCATE_END), 3);
	fListView->AddColumn(new BStringColumn(kLVTabNames[4], 100, 120, 0, B_TRUNCATE_END), 4);
	/*
	BStringColumn *cpuColumn = new BStringColumn(kLVTabNames[5], 0, 0, 0, B_TRUNCATE_END);
	fListView->AddColumn(cpuColumn, 5);
	cpuColumn->SetVisible(false);
	cpuColumn = new BStringColumn(kLVTabNames[6], 0, 0, 0, B_TRUNCATE_END);	
	fListView->AddColumn(cpuColumn, 6);
	cpuColumn->SetVisible(false);
	cpuColumn = new BStringColumn(kLVTabNames[7], 0, 0, 0, B_TRUNCATE_END);	
	fListView->AddColumn(cpuColumn, 7);
	cpuColumn->SetVisible(false);
	cpuColumn = new BStringColumn(kLVTabNames[8], 0, 0, 0, B_TRUNCATE_END);	
	fListView->AddColumn(cpuColumn, 8);
	cpuColumn->SetVisible(false);
	cpuColumn = new BStringColumn(kLVTabNames[9], 0, 0, 0, B_TRUNCATE_END);	
	fListView->AddColumn(cpuColumn, 9);
	cpuColumn->SetVisible(false);
	cpuColumn = new BStringColumn(kLVTabNames[10], 0, 0, 0, B_TRUNCATE_END);	
	fListView->AddColumn(cpuColumn, 10);
	cpuColumn->SetVisible(false);
	cpuColumn = new BStringColumn(kLVTabNames[11], 0, 0, 0, B_TRUNCATE_END);	
	fListView->AddColumn(cpuColumn, 11);
	cpuColumn->SetVisible(false);
	cpuColumn = new BStringColumn(kLVTabNames[12], 0, 0, 0, B_TRUNCATE_END);	
	fListView->AddColumn(cpuColumn, 12);
	cpuColumn->SetVisible(false);
	cpuColumn = new BStringColumn(kLVTabNames[13], 0, 0, 0, B_TRUNCATE_END);	
	fListView->AddColumn(cpuColumn, 13);
	cpuColumn->SetVisible(false);
	*/
	fListBox = new BBox(boxRect, "List Box", B_FOLLOW_ALL_SIDES);
	fListBox->AddChild(fListView);
	fListBox->SetLabel("Games");
	
	/* PlayButton */
	BRect buttonRect(boxRect);
	buttonRect.left -= 3;
	buttonRect.right -= 17;	
	buttonRect.bottom = boxRect.bottom - 20;
	buttonRect.top = buttonRect.bottom - 30;
	
	fPlayButton = new BButton(buttonRect, "Play Button", "Play",
		new BMessage(kPlay), B_FOLLOW_BOTTOM|B_FOLLOW_LEFT_RIGHT);
	fListBox->AddChild(fPlayButton);
	
	fBackgroundView->AddChild(fListBox);
			
	fListView->SetSelectionMessage(new BMessage(kLVSelectionChanged));
	fListView->SetMessage(new BMessage(kPlay));
	fListView->SetTarget(this);
	
	BRect contentRect(fTabs->ContainerView()->Bounds());
	fTabs->AddTab(new BitmapView(contentRect));
	//fTabs->AddTab(new BView(contentRect, "Game Settings", B_FOLLOW_ALL_SIDES, 0));	
	//fTabs->AddTab(new InfoView(contentRect));	
	fTabs->AddTab(new VerifyView(contentRect));
	fBackgroundView->AddChild(fTabs);
	
	/* Progress Bar */
	BRect statusRect;
	statusRect.left = fListBox->Frame().left;
	statusRect.right = fListBox->Frame().right;
	statusRect.top = fListBox->Frame().bottom;
	statusRect.bottom = Bounds().bottom - 20;
	
	fStatus = new BStatusBar(statusRect, "Progress Bar");
	fBackgroundView->AddChild(fStatus);
	fStatus->Hide();
	
	fListView->LoadState(Settings().ListViewState());
	
	AddChild(fBackgroundView);
}


void
BeMamerWindow::Show()
{
	if (fFirstShow) {
		fFirstShow = false;
		if (Settings().IsExpanded())
			PostMessage(kExpandListView);			
		PostMessage(kGenerateRomList);		
	}
	
	BWindow::Show();
}


bool
BeMamerWindow::QuitRequested()
{
	BAlert *alert = new BAlert("YO!", "Are you sure you want to quit?",
		"Yes", "No");
	int32 button = alert->Go();
	
	if (button == 0) {		
		fKillThread = true;
		status_t retVal;
		if (fRomGenerator > 0) {
			UnlockLooper();
			printf("Waiting for rom generator thread to quit...\n");
			wait_for_thread(fRomGenerator, &retVal);
			printf("\tdone!\n");
			bool result = LockLooper();
			printf("locked ? %s\n", result ? "true" : "false");
		}
		
		if (fRomChecker > 0) {
			// fRomChecker thread is waiting on our looper, so we unlock it temporarily
			UnlockLooper();
			printf("Waiting for rom checker thread to quit...\n");
			wait_for_thread(fRomChecker, &retVal);
			// re-lock the looper, as QuitRequested() requires.
			bool result = LockLooper();
			printf("locked ? %s\n", result ? "true" : "false");
		}
		
		Settings saveSettings;
		saveSettings.SetWindowFrame(Frame());
		saveSettings.SetLastSelectedGame(fListView->IndexOf(fListView->CurrentSelection()));
		saveSettings.SetExpanded(fLVIsExpanded);
		BMessage lvState;
		fListView->SaveState(&lvState);
		saveSettings.SetListViewState(&lvState);	
		Settings::Save();
				
		be_app->PostMessage(B_QUIT_REQUESTED);
		return BWindow::QuitRequested();
	} else
		return false;
}


void
BeMamerWindow::MessageReceived(BMessage *message)
{
	switch(message->what) {
		case kGenerateRomList:
		{
			printf("kGenerateRomList\n");
			status_t retVal;
			fKillThread = true;	
			if (fRomGenerator >= 0)
				wait_for_thread(fRomGenerator, &retVal);
			fKillThread = false;	
			fRomGenerator = spawn_thread((thread_entry)CreateGameList, "RomList Generator",
				B_NORMAL_PRIORITY, this);
				
			resume_thread(fRomGenerator);
			break;
		}
		case kCheckRoms:
		{
			status_t retVal;
			fKillThread = true;
			if (fRomChecker >= 0)
				wait_for_thread(fRomChecker, &retVal);
			fKillThread = false;
			fStatus->Show();
			fRomChecker = spawn_thread((thread_entry)CheckRoms, "Rom Checker", B_NORMAL_PRIORITY, this);
			resume_thread(fRomChecker);
			break;
		}
		case kPlay:
			Play();
			break;
		case kVerifyRomset:
		{
			void *from;
			if (message->FindPointer("source", &from) == B_OK) {
				BTextView *report = dynamic_cast<BTextView *>(FindView("Report"));				
				if (report) {
					TMameInterface mame(Settings().MamePath().Path());
					if (mame.InitCheck() == B_OK) {
						const char *romName = static_cast<BStringField *>(fListView->CurrentSelection()->GetField(1))->String();
						mame.MoreCheckSet(romName, report);
					} 
				}
			}
			break;
		}
		case kLVAddRemoveTab:
		{	
			BMenuItem *from;
			if (message->FindPointer("source", (void **)&from) == B_OK) {				
				fListView->ColumnAt(from->Menu()->IndexOf(from))->SetVisible(!from->IsMarked());
			}
			break;
		}
		case kLVSelectionChanged:
		case kTabChanged:
		{				
			BRow *selectedRow = fListView->CurrentSelection();
			if (selectedRow == NULL)
				break;
			message->AddString("romName", static_cast<BStringField *>(selectedRow->GetField(1))->String());
			message->AddString("cloneOf", static_cast<BStringField *>(selectedRow->GetField(3))->String());
			message->AddString("gameName", static_cast<BStringField *>(selectedRow->GetField(0))->String());
			/*message->AddString("cpu1", static_cast<BStringField *>(selectedRow->GetField(5))->String());
			message->AddString("cpu2", static_cast<BStringField *>(selectedRow->GetField(6))->String());
			message->AddString("cpu3", static_cast<BStringField *>(selectedRow->GetField(7))->String());
			message->AddString("cpu4", static_cast<BStringField *>(selectedRow->GetField(8))->String());
			message->AddString("sound1", static_cast<BStringField *>(selectedRow->GetField(9))->String());
			message->AddString("sound2", static_cast<BStringField *>(selectedRow->GetField(10))->String());
			message->AddString("sound3", static_cast<BStringField *>(selectedRow->GetField(11))->String());
			message->AddString("sound4", static_cast<BStringField *>(selectedRow->GetField(12))->String());
			*/
			/*message->AddString("control", selectedItem->control);
			message->AddInt32("numButtons", selectedItem->numButtons);
			message->AddInt32("numPlayers", selectedItem->numPlayers);
			*/
			PostMessage(message, fTabs->ViewForTab(fTabs->Selection()));
					
			BString newLabel("Play ");			
			newLabel.Append(static_cast<BStringField *>(selectedRow->GetField(0))->String());
			fPlayButton->SetLabel(newLabel.String());
			break;
		}
		case kViewFilter:
		{
			BMenuItem *source = NULL;
			message->FindPointer("source", (void **)&source);
			switch(source->Menu()->IndexOf(source)) {
				case 0:				
					//fListView->RemoveItems(0, fListView->CountItems());
					for (int32 x = 0; x < fNotAvailable.CountItems(); x++)
						fListView->AddRow((BRow*)fNotAvailable.ItemAtFast(x));					
					break;
				case 1:
					for (int32 x = fListView->CountRows() - 1; x >=0; x--) {
						BRow *row = fListView->RowAt(x);
						if (!fAvailable.HasItem(row)) {
							fListView->RemoveRow(row);
							fNotAvailable.AddItem(row);
						}						
					}
					break;	
				case 2:					
					//fListView->RemoveItems(0, fListView->CountItems());
					//fListView->AddList(&fFavorites);					
					break;
				default:
					break;
			}			
			break;
		}
		case kExpandListView:
		{
			if (fLVIsExpanded) {
				fLVIsExpanded = false;
				fTabs->Show();
				BRect rect(fListBox->Bounds());
				fListBox->ResizeTo(rect.Width() - fTabs->Frame().Width() - 20, fListBox->Bounds().Height()); 								
			} else {
				fLVIsExpanded = true;
				fTabs->Hide();
				fListBox->ResizeTo(fBackgroundView->Bounds().InsetBySelf(10, 0).Width(),
						fListBox->Bounds().Height());
				for (int32 x = 0; x < fListView->CountColumns(); x++)
					fListView->ColumnAt(x)->SetVisible(true);			
			}
			BMenuBar *menuBar = KeyMenuBar();
			if (menuBar) {
				BMenuItem *item = menuBar->FindItem("Columns");
				if (item)
					item->SetEnabled(!fLVIsExpanded);
			}
			fPlayButton->Invalidate();
			break;
		}
		default:
			BWindow::MessageReceived(message);
			break;
	}
}


void
BeMamerWindow::MenusBeginning()
{
	BMenuBar *menuBar = KeyMenuBar();
	if (menuBar != NULL) {
		BMenu *columnsMenu = menuBar->SubmenuAt(2);
		if (columnsMenu != NULL) {
			columnsMenu->RemoveItems(0, columnsMenu->CountItems(), true);
			for (int32 x = 0; x < fListView->CountColumns(); x++) {
				BColumn *column = fListView->ColumnAt(x);
				if (column != NULL) {
					BString columnName;
					column->GetColumnName(&columnName);
					BMessage *menuMessage = new BMessage(kLVAddRemoveTab);
					BMenuItem *menuItem = new BMenuItem(columnName.String(), menuMessage);
					columnsMenu->AddItem(menuItem);
					menuItem->SetMarked(column->IsVisible());					
				}
			}
		}
		BMenu *viewMenu = menuBar->SubmenuAt(1);
		if (viewMenu != NULL) {
			BMenuItem *item = viewMenu->FindItem(kExpandListView);
			if (item)
				item->SetMarked(fLVIsExpanded);
		}				
	}
	BWindow::MenusBeginning();
}


float
BeMamerWindow::AddMenuBar()
{
	// menuBar
	BMenuBar *menuBar = new BMenuBar(BRect(0, 0, 0, 0), "menuBar", 
				B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP, B_ITEMS_IN_ROW, true);
	
	// Create menu
	BMenu *menu = new BMenu("File");
	menu->AddItem(new BMenuItem("Refresh Rom List", new BMessage(kGenerateRomList)));
	menu->AddItem(new BMenuItem("Check Available Roms", new BMessage(kCheckRoms)));
	menu->AddItem(new BMenuItem("Options", new BMessage(kOpenPrefsWindow), 'P'));
	menu->FindItem("Options")->SetTarget(be_app);
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q'));
	menuBar->AddItem(menu);
		
	menu = new BMenu("View");
	menu->AddItem(new BMenuItem("Expanded", new BMessage(kExpandListView)));
	menuBar->AddItem(menu);
	/*
	menu = new BMenu("Filter");
	menu->AddItem(new BMenuItem("All", new BMessage(kViewFilter)));
	menu->AddItem(new BMenuItem("Available", new BMessage(kViewFilter)));
	menu->AddItem(new BMenuItem("Favorites", new BMessage(kViewFilter)));
	menu->SetRadioMode(true);
	//menu->ItemAt(Settings().ViewFilter())->SetMarked(true);
	menu->ItemAt(0)->SetMarked(true);
	menuBar->AddItem(menu);
	*/
	menu = new BMenu("Columns");
	menuBar->AddItem(menu);
				
	menu = new BMenu("Help");
	menu->AddItem(new BMenuItem("About", new BMessage(B_ABOUT_REQUESTED)));
	menu->SetTargetForItems(be_app);
	menuBar->AddItem(menu);
	
	AddChild(menuBar); // Add the full menuBar
	
	return menuBar->Bounds().Height();	
}


int32
BeMamerWindow::CreateGameList(void *castToThis)
{
	BeMamerWindow *object = static_cast<BeMamerWindow *>(castToThis);
	
	if (object->LockLooper()) {
		object->fListView->Clear();
	   	object->UnlockLooper();
	}	     			
	BPath settingsPath(Settings().Path());
	settingsPath.Append("gamelist");
	
	BEntry entry;
	entry.SetTo(settingsPath.Path());
		
	BPath mamePath(Settings().MamePath());
	printf("mamePath: %s\n", mamePath.Path());
	
   	status_t result = B_ERROR;
   	TMameInterface mame(mamePath.Path(), Settings().Path().Path());
   	
   	if (mame.InitCheck() == B_OK || entry.Exists()) {
		Rom rom;
		while (mame.GetNextRom(&rom) == B_OK && !object->fKillThread) {
			BRow *row = new BRow();
			BStringField *description = new BStringField(rom.description);
        	BStringField *romName = new BStringField(rom.romName);
        	BStringField *year = new BStringField(rom.year);
        	BStringField *cloneOf = new BStringField(rom.cloneOf);
        	BStringField *manufacturer = new BStringField(rom.manufacturer);
        	
        	/*BStringField *cpu1 = new BStringField(rom.cpu1);
        	BStringField *cpu2 = new BStringField(rom.cpu2);
        	BStringField *cpu3 = new BStringField(rom.cpu3);
        	BStringField *cpu4 = new BStringField(rom.cpu4);
        	BStringField *sound1 = new BStringField(rom.sound1);
        	BStringField *sound2 = new BStringField(rom.sound2);
        	BStringField *sound3 = new BStringField(rom.sound3);
        	BStringField *sound4 = new BStringField(rom.sound4);
        	*/
			row->SetField(description, 0);
			row->SetField(romName, 1);
			row->SetField(year, 2);
			row->SetField(cloneOf, 3);
			row->SetField(manufacturer, 4);
			/*
			row->SetField(cpu1, 5);
			row->SetField(cpu1, 6);
			row->SetField(cpu1, 7);
			row->SetField(cpu1, 8);
			row->SetField(sound1, 9);
			row->SetField(sound2, 10);
			row->SetField(sound3, 11);
			row->SetField(sound4, 12);
			*/
			if (object->LockLooper()) {
				object->fListView->AddRow(row);
				object->UnlockLooper();
			}
			rom.Clear();
		}
		
		if (object->LockLooper()) {
			BString title("BeMAMEr - ");
    		title << object->fListView->CountRows() << " games";
   	 		object->SetTitle(title.String());
   		 	object->fListView->AddToSelection(object->fListView->RowAt(Settings().LastSelectedGame()));
   		 	object->UnlockLooper();
   		 	result = B_OK;
   		}
	}
                       
    return result;
}


int32
BeMamerWindow::CheckRoms(void *castToThis)
{
	BeMamerWindow *object = static_cast<BeMamerWindow *>(castToThis);
	int32 itemCount = 0;
	object->fAvailable.MakeEmpty();
	
	TMameInterface mame(Settings().MamePath().Path());
	status_t result = mame.InitCheck();
	if (result != B_OK)
		return result;
		
	if (object->LockLooper()) { 
		itemCount = object->fListView->CountRows();
		object->fStatus->Reset("Checking roms...");
		object->fStatus->SetMaxValue(itemCount);
		object->UnlockLooper();
	}
	
	for (int32 x = 0; x < itemCount; x++) {
		if (object->fKillThread) {
			printf("Killing_thread\n");
			break;
		}
		BString commandLine;
		BRow *rowAt = NULL;
		if (object->LockLooper()) {
			rowAt = object->fListView->RowAt(x);
			BStringField *item = static_cast<BStringField *>(rowAt->GetField(1));
			const char *romName = item->String();
			commandLine << romName;
			object->UnlockLooper();
		}
		
		printf("commandLine: %s\n", commandLine.String());
			
		result = mame.CheckSet(commandLine.String());
		if (object->LockLooper()) {
			object->fStatus->Update(1.0);
			if (result == B_OK)
				object->fAvailable.AddItem(rowAt);
				
			//item->SetAvailable(result == B_OK);
						
			object->UnlockLooper();
			snooze(200);
		}
	}
		
	return result;
}


status_t
BeMamerWindow::Play()
{
	TMameInterface mame(Settings().MamePath().Path());
	status_t result = mame.InitCheck();
	if (result != B_OK)
		return result;
	BRow *selectedRow = fListView->CurrentSelection();
	if (selectedRow == NULL)
		return B_ERROR;
		
	BString commandline;
	commandline << static_cast<BStringField *>(selectedRow->GetField(1))->String();
	
	BString output;
	result = mame.Play(commandline.String(), &output);
	
	if (result != B_OK) {
		TGameAlert *alert = new TGameAlert(output);
		alert->Show();
	}
		
	return result;
}


void
BeMamerWindow::SelectRandomGame()
{
	/*int32 numItems = fListView->CountRows();
	uint32 index = 0; //Dice(numItems).Roll();
	fListView->SetFocusRow(index, true);*/
}
