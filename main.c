#constant OVERVIEW_FORM 0
#constant SHELF_FORM 1
#constant MOD_FORM 2

// Shelves have 8 major characteristics
// 1) Total Voltage, Float: 2 Bytes
// 2) Avg. Temp, Int: 1 Byte
// 3) Highest Temp, Float: 1 Byte
// 4) Lowest Temp, Float: 1 Bytes
// 5) ID Highest/Lowest Temp, Int: 1 Byte
var shelf1[8];
var shelf2[8];
var shelf3[8];

// Modules have 3 major characteristics
// 1) Voltage, Float: 2 Bytes
// 2) Temperature, Int: 1 Byte
// 3) Alarm/Error, Int: 1 Byte
var mod0[4];
var mod1[4];
var mod2[4];
var mod3[4];
var mod4[4];
var mod5[4];
var mod6[4];
var mod7[4];
var mod8[4];
var mod9[4];
var mod10[4];
var mod11[4];

func print_voltage(upper, lower, x_pos, y_pos)

    // Setting up float arrays
    var volt_int, volt_flt[2], temp_flt[2];
    
    // Do the bit shift for 16bit int
    volt_int = (upper << 8) + lower;

    // Convert to float
    flt_ITOF(volt_flt, volt_int);

    // Create multiplication factor
    flt_VAL(temp_flt, "0.02");
    
    // (voltage = [0xUpperByte,0xLowerByte] * 0.02)
    flt_MUL(volt_flt, volt_flt, temp_flt);

    // Print it at desired location
    gfx_MoveTo(x_pos, y_pos);
    flt_PRINT(volt_flt, "%-.0f");

endfunc

func process_input()

    var tmpByte;
    var dataByteCount := 0x0F;
    var tempVals[15];

    if(com1_Count() > dataByteCount)
        
        // Read in the first byte
        // Should be START BYTE (0xAA)
        tmpByte := serin1();

        // Looking for the start of a message so...
        // While tmpByte isn't the Start Byte
        while(tmpByte != START_BYT)

            // If there isn't enough data
            // in the buffer for a full message
            if(com1_Count() < dataByteCount)
                comCount := com1_Count();
                return; // Leave function
            endif

            // Read in the next byte
            tmpByte := serin1();

        wend

        // Give it  5msec to breath
        pause(5);

        // Read in vals from buffer into temp array
        for(i := 0; i < dataByteCount; i++)
            tempVals[i] := serin1();
        next

        // Init check sum calc
        // var checksum := START_BYT;
        // for(i := 0; i < 0x0E; i++)
        //     checksum += tempVals[i];
        //     checksum := checksum & 0xFF;
        // next

        // Check the checksum
        // if( checksum != tempVals[14])
        //     return; // Leave Function
        // endif
        
        txt_Set(TEXT_HIGHLIGHT, 0xFFFF);
        txt_Set(TEXT_COLOUR, BLACK);
        
        if(current_form == OVERVIEW_Form)
            
            switch(tempVals[0])

            endswitch
        endif

        if(current_form == SHELF_FORM)
            switch(tempVals[0])

            endswitch
        endif

        if(current_form == MOD_FORM)
            switch(tempVals[0])

            endswitch
        endif
    endif
endfunc