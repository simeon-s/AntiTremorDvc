// IMPORTANT: 
// Cell and sheets global definitions doesn't work if there are not declared as local variables
var defaultMotorsNum = 6;
var defaultImpulseDuration = 250;
var defaultCycleDuration = 2000;

var offsetRows = 7; // How many rows below are the 0 and 1
var app;
var targetSheet;
var loopStep;
var loopCycle;
var loopStepsCount;
var SS = SpreadsheetApp.openById('1nUoDiR7xj8X7W2oRtzgT2SJqzAmHJTq4JYrItQ77hoI');
var sheetV = SS.getSheetByName('01. Control'); // second sheetVariable
var str = "";
var cellToMCU = 'O34'; //'O33'; // B33

function updateCells() {
    ColorizeRows();
    var sh = SpreadsheetApp.getActive().getSheetByName("01. Control");
    var values = sh.getRange('B7:K12').getValues().map(r => r.map(c => c ? 1 : 0));
    sh.getRange(14, 2, values.length, values[0].length).setValues(values);
    binaryStringToInt();
}


function Update() {
    ColorizeRows();
    var rowsNumber = 6;
    var columnsNumber = 10;
    var rowsStart = 7; // starting row number
    var columnsStart = 2; // starting column number
    var i = rowsStart;
    var j = columnsStart;
    var valueFromCurrentCell = 0;



    app = SpreadsheetApp;
    targetSheet = app.getActiveSpreadsheet().getSheetByName("01. Control"); // sheet by name
    //  readFromSheet()

    for (var i = rowsStart; i < rowsStart + rowsNumber; i++) {
        for (var j = columnsStart; j < columnsStart + columnsNumber; j++) {
            var readVal = targetSheet.getRange(i, j).getValue();
            var writeVal;
            var cell = targetSheet.getRange(i, j);
            writeVal = cell.offset(7, 0);
            if (readVal == true)
                writeVal.setValue(1);
            else
                writeVal.setValue(0);
        }
    }

    binaryStringToInt();
}




function resetCheckBox() {
    readFromSheet();
    var i = 0;
    var j = 0;
    var rowsNumber = 6;
    var columnsNumber = 10;
    var rowsStart = 7; // starting row number
    var columnsStart = 2; // starting column number

    for (var i = rowsStart; i < rowsStart + rowsNumber; i++) {
        for (var j = columnsStart; j < columnsStart + columnsNumber; j++) {
            var setVal = targetSheet.getRange(i, j).setValue(false);
        }
    }
    rowsStart += offsetRows;
    for (var i = rowsStart; i < rowsStart + rowsNumber; i++) {
        for (var j = columnsStart; j < columnsStart + columnsNumber; j++) {
            var setVal = targetSheet.getRange(i, j).setValue(0);

        }
    }
    updateCells();
}

function readFromSheet() {
    app = SpreadsheetApp;
    targetSheet = app.getActiveSpreadsheet().getSheetByName("01. Control"); // sheet by name
    loopStep = targetSheet.getRange("B2").getValue();
    loopCycle = targetSheet.getRange("B3").getValue();
    loopStepsCount = loopCycle / loopStep;

}

function ColorizeRows() { // Colorizes the checkbox
    var startingRow = 7;
    var startingColumn = 2;
    var maxStepsInCycle = 10;
    var sheet = SpreadsheetApp.getActiveSheet();
    var activeRange = SpreadsheetApp.getActiveRange();
    var loopSteps = sheet.getRange("B4").getValue(); 
    if (loopSteps > maxStepsInCycle)
        loopSteps = maxStepsInCycle;
    var actuatorNum = sheet.getRange("B1").getValue(); 
    var lastColumn = 12; //sheet.getLastColumn();
    lastColumn = lastColumn - (lastColumn - loopSteps);
    var changeRange = sheet.getRange(7, 2, 1, lastColumn);
    for (var currentRow = startingRow; currentRow < startingRow + actuatorNum; currentRow++) {
        changeRange = sheet.getRange(currentRow, startingColumn, 1, maxStepsInCycle);
        changeRange.setBackgroundRGB(224, 222, 222);

    }
    for (var currentRow = startingRow; currentRow < startingRow + actuatorNum; currentRow++) {
        changeRange = sheet.getRange(currentRow, startingColumn, 1, lastColumn);
        changeRange.setBackgroundRGB(130, 180, 40);

    }

}


function ResetDefaults() {

    app = SpreadsheetApp;
    targetSheet = app.getActiveSpreadsheet().getSheetByName("01. Control"); // sheet by name
    targetSheet.getRange("B1").setValue(defaultMotorsNum); 
    targetSheet.getRange("B2").setValue(defaultImpulseDuration); 
    targetSheet.getRange("B3").setValue(defaultCycleDuration); 
    resetCheckBox();
    ColorizeRows();
}

function binaryStringToInt() {



    var binaryStringFromCell;
    var transformedInt;
    app = SpreadsheetApp;
    targetSheet = app.getActiveSpreadsheet().getSheetByName("01. Control");
    binaryStringFromCell = targetSheet.getRange("L14").getValue();
    transformedInt = parseInt(binaryStringFromCell, 2);
    targetSheet.getRange("M14").setValue(transformedInt);
    binaryStringFromCell = targetSheet.getRange("L15").getValue();
    transformedInt = parseInt(binaryStringFromCell, 2);
    targetSheet.getRange("M15").setValue(transformedInt);
    binaryStringFromCell = targetSheet.getRange("L16").getValue();
    transformedInt = parseInt(binaryStringFromCell, 2);
    targetSheet.getRange("M16").setValue(transformedInt);
    binaryStringFromCell = targetSheet.getRange("L17").getValue();
    transformedInt = parseInt(binaryStringFromCell, 2);
    targetSheet.getRange("M17").setValue(transformedInt);
    binaryStringFromCell = targetSheet.getRange("L18").getValue();
    transformedInt = parseInt(binaryStringFromCell, 2);
    targetSheet.getRange("M18").setValue(transformedInt);
    binaryStringFromCell = targetSheet.getRange("L19").getValue();
    transformedInt = parseInt(binaryStringFromCell, 2);
    targetSheet.getRange("M19").setValue(transformedInt);

    /* Doesn't work. "#NUM!" error. Can't fix it.
    var rowsStart = 14; // starting row number
    var column = 13; // column number for 'L'
    for(var i = rowsStart; i<rowsStart+6; i++)
    {
       //var cell = targetSheet.getRange(i,j);
      var binaryStringFromCell = targetSheet.getRange(i, column).getValue(); 
      var transformedInt = parseInt(binaryStringFromCell, 2 );
      transformedInt = Number(transformedInt);
      targetSheet.getRange(i, column).setValue(i); // pisane v kletkavvv
    }
    */




}


//------------------------------------------------------------------------------------------



function onOpen() {
    var ui = SpreadsheetApp.getUi();
    ui.createMenu('ESP8266 Logging')
        .addItem('Clear', 'Clear')
        .addToUi();
}

function Clear() {
    sheetV.deleteRows(4, sheetV.getLastRow());
    SS.toast('Chart cleared', 'ESP8266 logging', 5);
}


function doPost(e) {

    var parsedData;
    var result = {};

    try {
        parsedData = JSON.parse(e.postData.contents);
    } catch (f) {
        return ContentService.createTextOutput("Error in parsing request body: " + f.message);
    }

    if (parsedData !== undefined) {
        // Common items first
        // data format: 0 = display value(literal), 1 = object value
        var flag = parsedData.format;

        if (flag === undefined) {
            flag = 0;
        }

        switch (parsedData.command) {
            case "appendRow":
                var tmp = SS.getSheetByName(parsedData.sheet_name);
                var nextFreeRow = tmp.getLastRow() + 1;
                var dataArr = parsedData.values.split(",");

                tmp.appendRow(dataArr);

                str = "Success";
                SpreadsheetApp.flush();
                break;


        }

        return ContentService.createTextOutput(str);
    } // endif (parsedData !== undefined)
    else {
        return ContentService.createTextOutput("Error! Request body empty or in incorrect format.");
    }


}


function doGet(e) {

    var val = e.parameter.value;
    var cal = e.parameter.cal;
    var read = e.parameter.read;

    if (cal !== undefined) {
        return ContentService.createTextOutput(GetEventsOneWeek());
    }

    if (read !== undefined) {
        var now = Utilities.formatDate(new Date(), "Etc/GMT", "yyyy-MM-dd'T'hh:mm a'Z'").slice(11, 19);
        sheetV.getRange('B22').setValue(now);
        var count = (sheetV.getRange('B23').getValue()) + 1;
        sheetV.getRange('B23').setValue(count);
        return ContentService.createTextOutput(sheetV.getRange(cellToMCU).getValue()); // GET DATA CEL!!!!!!!!!!!
    }

    if (e.parameter.value === undefined)
        return ContentService.createTextOutput("No value passed as argument to script Url.");

    var range = sheetV.getRange('A1');
    var retval = range.setValue(val).getValue();
    var now = Utilities.formatDate(new Date(), "EST", "yyyy-MM-dd'T'hh:mm a'Z'").slice(11, 19);
    //sheetV.getRange('B1').setValue(now);
    //sheetV.getRange('C1').setValue('0');
    if (retval == e.parameter.value)
        return ContentService.createTextOutput("Successfully wrote: " + e.parameter.value + "\ninto spreadsheet.");
    else
        return ContentService.createTextOutput("Unable to write into spreadsheet.\nCheck authentication and make sure the cursor is not on cell 'A1'." + retval + ' ' + e.parameter.value);
}

function GetEventsOneWeek() {
    var Cal = CalendarApp.getCalendarsByName('Test REST API')[0];
    // Need to create 2 separate Date() objects. Cannot do 'OneWeekFromNow = Nowjs' to 
    // simply get it's value and use that later without modifying 'Now'
    // since in JS, an object is automatically passed by reference
    var Now = new Date();
    var OneWeekFromNow = new Date();
    OneWeekFromNow.setDate(Now.getDate() + 7);
    //Logger.log(Now);
    //Logger.log(OneWeekFromNow);
    var events = Cal.getEvents(Now, OneWeekFromNow);
    //Logger.log(events.length);
    var str = '\nEvent Title,\tDescription,\tRecurring?,\tAll-day?,\tFirst Reminder (in minutes before event)\n';
    for (var i = 0; i < events.length; i++) {
        str += events[i].getTitle() + ',\t' + events[i].getDescription() + ',\t' + events[i].isRecurringEvent() + ',\t' + events[i].isAllDayEvent() + ',\t' + events[i].getPopupReminders()[0];
        str += '\n';
    }
    //Logger.log(str);
    return str;
}
