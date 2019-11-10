#constant OVERVIEW_FORM 0
#constant SHELF_FORM 1
#constant MOD_FORM 2
var current_form := OVERVIEW_FORM;
var prev_form := OVERVIEW_FORM;

#constant TYPE  2
#constant ID    3
#constant MOD_V 4
#constant MOD_T 6
#constant MOD_A 7

#constant SHELF_V
#constant SHELF_T
#constant SHELF_H
#constant SHELF_L
#constant SHELF_ID
#constant SHELF_A
#constant SHELF_E

// Shelves have 8 major characteristics
// 1) Total Voltage, Float: 2 Bytes
// 2) Avg. Temp, Int: 1 Byte
// 3) Highest Temp, Float: 1 Byte
// 4) Lowest Temp, Float: 1 Bytes
// 5) ID Highest/Lowest Temp, Int: 1 Byte
var shelf0[8];
var shelf1[8];
var shelf2[8];
var shelves[3] := [shelf0, shelf1, shelf2];
var current_shelf := 0;
var *shelf_ptr;

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
var mod12[4];
var mod13[4];
var mod14[4];
var mod15[4];
var mod16[4];
var mod17[4];
var mod18[4];
var mod19[4];
var mod20[4];
var mod21[4];
var mod22[4];
var mod23[4];
var mod24[4];
var mod25[4];
var mod26[4];
var mod27[4];
var mod28[4];
var mod29[4];
var mod30[4];
var mod31[4];
var mod32[4];
var mod33[4];
var mod34[4];
var mod35[4];
var mods[36] := [mod0, mod1, mod2, mod3, mod4, mod5, mod6, mod7, mod8, mod9, mod10, mod11, mod12, mod13, mod14, mod15, mod16, mod17, mod18, mod19, mod20, mod21, mod22, mod23, mod24, mod25, mod26, mod27, mod28, mod29, mod30, mod31, mod32, mod33, mod34, mod35];
var current_mod := 0;
var *mod_ptr_1;
var *mod_ptr_2;

func main()

    // Mount the drive

    // Basic initialization stuff

    // Enter the forever loop
    repeat

        // Send Commands

        // Read Commands

        // Update the view

        // Check for user input

        // React to user input

    forever

endfunc

func print_voltage(var volt_int, var x_pos, var y_pos)

    // Setting up float arrays
    var volt_flt[2], temp_flt[2];
    
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

func print_temperature(var temp_int, var x_pos, var y_pos)

    var temp_flt_1[2], temp_flt_2[2];

    // Get the temperature
    flt_VAL(temp_flt_1, "0.5");
    flt_ITOF(temp_flt_2, temp_int);
    flt_MUL(temp_flt_1, temp_flt_1, temp_flt_2);

    gfx_MoveTo(x_pos, y_pos);
    flt_PRINT(temp_flt_1, "%-.0f");

endfunc

func process_input()

    var tmpByte;
    var dataByteCount := 0x0F;
    var temp_vals[15];
    var shelf_number;
    var mod_number;

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
            temp_vals[i] := serin1();
        next
        
        txt_Set(TEXT_HIGHLIGHT, 0xFFFF);
        txt_Set(TEXT_COLOUR, BLACK);
        
        if(current_form == OVERVIEW_Form)
            switch(temp_vals[0])

            endswitch
        endif

        // If it's a shelf update message
        if(temp_vals[TYPE] == 0xA0)
            
            // See who the message is for
            shelf_number := temp_vals[ID];

            // Get their attention
            get_shelf(shelf_number);

            // Update their info
            update_shelf(temp_vals);
        endif

        // If it's a mod update message
        if((temp_vals[TYPE] & 0xB0) == 0xB0)
            
            // See who the message is for
            shelf_number := temp_vals[ID];
            mod_number := ((temp_vals[TYPE] | 0xF0) & 0x0F)
            
            // Get their attention
            get_mod(shelf_number, mod_number);
            
            // Update their info
            update_mods(temp_vals);          
        endif
    endif
endfunc

func get_shelf(var shelf)

    shelf_ptr := shelves[shelf];

endfunc

func update_shelf(var data)

    shelf_ptr[0] := data[SHELF_V];
    shelf_ptr[1] := data[SHELF_V + 1];
    shelf_ptr[2] := data[SHELF_T];
    shelf_ptr[3] := data[SHELF_H];
    shelf_ptr[4] := data[SHELF_L];
    shelf_ptr[5] := data[SHELF_ID];
    shelf_ptr[6] := data[SHELF_A];
    shelf_ptr[7] := data[SHELF_E];

endfunc

func request_mod(var shelf, var mod)

    var cmd[7];
    cmd[0] := 0x18;
    cmd[1] := 0xEA;
    cmd[2] := shelf;
    cmd[3] := 0x0F;
    cmd[4] := (0xB0) + (mod/2);
    cmd[5] := 0xFF;
    cmd[6] := 0x00;

    var i;
    for(i := 0; i < 7; i++)
        serout1(cmd[i]);
    next

endfunc

func get_mod(var shelf, var mod)

    var mods_per_shelf := 12;
    mod_ptr_1 := mods[mod + (shelf * mods_per_shelf)];
    mod_ptr_2 := mods[mod + (shelf * mods_per_shelf) + 1];

endfunc

func update_mods(var data)
    
    // Get the voltage
    mod_ptr_1[0] := data[MOD_V];
    mod_ptr_1[1] := data[MOD_V + 1];
    mod_ptr_2[0] := data[MOD_V + 4];
    mod_ptr_2[1] := data[MOD_V + 4 + 1];
    
    // Get the temperature
    mod_ptr_1[2] := data[MOD_T];
    mod_ptr_2[2] := data[MOD_T + 4];
    
    // Get the Alarm/Error
    mod_ptr_1[3] := data[MOD_A];
    mod_ptr_2[3] := data[MOD_A + 4];
endfunc