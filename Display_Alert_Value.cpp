// The top of every source code file must include this line
#include "sierrachart.h"

// For reference, refer to this page:
// https://www.sierrachart.com/index.php?page=doc/AdvancedCustomStudyInterfaceAndLanguage.php

// This line is required. Change the text within the quote
// marks to what you want to name your group of custom studies. 
SCDLLName("Display_Alert_Value")

SCSFExport scsf_Display_Alert_Value(SCStudyInterfaceRef sc)
{
    SCInputRef alertcondition = sc.Input[0];
	SCInputRef dispvalue = sc.Input[1];
	SCInputRef displayPosition = sc.Input[2];
	SCInputRef displayoffset = sc.Input[3];
	SCInputRef textsize = sc.Input[4];
	SCInputRef textcolor = sc.Input[5];
	
	// Set configuration variables	
	if (sc.SetDefaults)
	{
		// Set the configuration and defaults		
		sc.GraphName = "Display_Alert_Value";		
		sc.StudyDescription = "";
		sc.GraphRegion = 0;
		
		alertcondition.Name = "Alert Condition";
		alertcondition.SetStudySubgraphValues(86,0);
		
		dispvalue.Name = "Display Value";
		dispvalue.SetStudySubgraphValues(23,0);
		
		displayPosition.Name = "Display Position";
		displayPosition.SetStudySubgraphValues(0,1);
		
		displayoffset.Name = "Display Offset";
		displayoffset.SetFloat(5);
		
		textsize.Name = "Text Size";
		textsize.SetInt(10);
		
		textcolor.Name = "Text Color";
		textcolor.SetColor(RGB(255,255,255));
				
		sc.AutoLoop = 1;
		sc.FreeDLL = 0;		
		return;
	}
	
	if (sc.GetBarHasClosedStatus() == BHCS_BAR_HAS_NOT_CLOSED)		return;
	
	// Do data processing	
	SCFloatArray alertArray;
	SCFloatArray dispvalueArray;
	SCFloatArray dispPosArray;
	
	sc.GetStudyArrayUsingID(alertcondition.GetStudyID(), alertcondition.GetSubgraphIndex(), alertArray);
	sc.GetStudyArrayUsingID(dispvalue.GetStudyID(), dispvalue.GetSubgraphIndex(), dispvalueArray);
	sc.GetStudyArrayUsingID(displayPosition.GetStudyID(), displayPosition.GetSubgraphIndex(), dispPosArray);
	
	if (alertArray.GetArraySize() == 0){
		sc.AddMessageToLog("alertArray values are null or zero!", 1);
		return;
	}
	if (dispvalueArray.GetArraySize() == 0){
		sc.AddMessageToLog("dispvalueArray values are null or zero!", 1);
		return;
	}
	
	for (int i = sc.UpdateStartIndex; i < sc.ArraySize; i++) {
		if(alertArray[i] != 0 && !sc.HideStudy) {
			int roundedValue = sc.Round(dispvalueArray[i]);	
			SCString s;
			s.Format("%d", roundedValue);
			s_UseTool TextDrawing;
            TextDrawing.Clear(); // Not necessary but good practice
            TextDrawing.ChartNumber = sc.ChartNumber;
			TextDrawing.LineNumber = sc.ChartNumber + i;
            TextDrawing.DrawingType = DRAWING_TEXT;			
            TextDrawing.BeginIndex = i;
            TextDrawing.BeginValue = dispPosArray[i] + displayoffset.GetFloat();
            TextDrawing.Color = textcolor.GetColor(); // Set text color
            TextDrawing.Text = s; // Your text here
			TextDrawing.TextAlignment = DT_CENTER;
            TextDrawing.FontSize = textsize.GetInt();; // Set font size
            TextDrawing.AddMethod = UTAM_ADD_ONLY_IF_NEW; // Add only if a new drawing is created
            sc.UseTool(TextDrawing);
		}
	}
}