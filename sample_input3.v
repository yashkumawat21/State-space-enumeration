module
    liftcontroller(start,clk,reset_inside_button,reset_up,reset_down,up_button,down_button,inside_button,present_floor);
    
    input start,clk;
    
    input [6:0] up_button,down_button,inside_button;
    
    output [6:0] reset_inside_button,reset_up,reset_down,present_floor;
    
    reg [6:0] reset_inside_button,reset_up,reset_down,present_floor;
    
    reg dir_up;
    
    reg [4:0] nstate;
    
    parameter 	s0_idle = 0,s0_up = 1,door_0 = 2,s1_idle = 3,s1_up = 4,s1_down = 5,
    door_1 = 6,s2_idle = 7,s2_up = 8,s2_down = 9,door_2 = 10,s3_idle = 11,s3_down = 12,
    door_3 = 13,s3_up = 14,door_4 = 15,door_5 = 16, door_6 = 17, s4_up = 18, s5_up = 19,
    s5_down = 20, s4_down = 21, s6_down = 22, s4_idle = 23, s5_idle = 24, s6_idle = 25;
    
    initial begin
        reset_inside_button <= 7'b0000000;
        reset_up            <= 7'b0000000;
        reset_down          <= 7'b0000000;
        present_floor       <= 7'b0000000;
    end
    

    	// All of the above logic is driven by the baud counter.  Bits must last
	// clocks_per_baud in length, and this baud counter is what we use to
	// make certain of that.
	//
	// The basic logic is this: at the beginning of a bit interval, start
	// the baud counter and set it to count clocks_per_baud.  When it gets
	// to zero, restart it.
	//



    
	// However, comparing a 28'bit number to zero can be rather complex--
	// especially if we wish to do anything else on that same clock.  For
	// that reason, we create "zero_baud_counter".  zero_baud_counter is
	// nothing more than a flag that is true anytime baud_counter is zero.
	// It's true when the logic (above) needs to step to the next bit.
	// Simple enough?
	//
	// I wish we could stop there, but there are some other (ugly)
	// conditions to deal with that offer exceptions to this basic logic.
	//
	// 1. When the user has commanded a BREAK across the line, we need to
	// wait several baud intervals following the break before we start
	// transmitting, to give any receiver a chance to recognize that we are
	// out of the break condition, and to know that the next bit will be
	// a stop bit.
	//
	// 2. A reset is similar to a break condition--on both we wait several
	// baud intervals before allowing a start bit.
	//
	// 3. In the idle state, we stop our counter--so that upon a request
	// to transmit when idle we can start transmitting immediately, rather
	// than waiting for the end of the next (fictitious and arbitrary) baud
	// interval.
	//
	// When (i_wr)&&(!r_busy)&&(state == TXU_IDLE) then we're not only in
	// the idle state, but we also just accepted a command to start writing
	// the next word.  At this point, the baud counter needs to be reset
	// to the number of clocks per baud, and zero_baud_counter set to zero.
	//
	// The logic is a bit twisted here, in that it will only check for the
	// above condition when zero_baud_counter is false--so as to make
    
    always @(posedge clk,start,nstate)

    begin

        if (start == 1'b1)
            nstate = s0_idle;

        else begin
            case(nstate)


                //------------------------------------------ground floor-----------------------------------------------
                s0_idle:begin
                    
                    present_floor = 7'd0;
                    if (up_button ! = 7'd0 || down_button ! = 7'd0 || inside_button ! = 7'd0)
                        nstate = s0_up;

                    else
                        nstate = s0_idle;
                    
                end
    
                s0_up:begin

                    present_floor          = 7'd0;
                    reset_up[0]            = 1'b0;
                    reset_inside_button[0] = 1'b0;

                    if (up_button[0] == 1'b1 || inside_button[0] == 1'b1)
                        nstate = door_0;
                    
                    else if ((up_button ! = 7'd0 || down_button ! = 7'd0 || inside_button ! = 7'd0) && up_button[0] == 1'b0 && inside_button[0] == 1'b0)
                    nstate = s1_up;

                    else
                    nstate = s0_idle;
                    
                end
                
                door_0:begin

                    reset_up[0]            = 1'b1;
                    reset_inside_button[0] = 1'b1;
                    nstate                 = s0_up;

                end
                
                //---------------------------------------------1st floor------------------------------------------------
                
                s1_idle:begin

                    present_floor = 7'b0000010;

                    if (up_button ! = 7'd0 || down_button ! = 7'd0 || inside_button ! = 7'd0) begin
                        if (up_button[0] == 1'b1 || inside_button[0] == 1'b1 || down_button[1] == 1'b1)
                            nstate = s1_down;

                        else
                            nstate = s1_up;

                    end

                    else
                    nstate = s1_idle;
                    
                
                end
                
                
                
                s1_up:begin

                    present_floor          = 7'b0000010;
                    reset_up[1]            = 1'b0;
                    reset_inside_button[1] = 1'b0;

                    if (up_button[1] == 1'b1 || inside_button[1] == 1'b1) begin
                        dir_up = 1'b1;
                        nstate = door_1;
                    end
                    
                    else if (up_button[5:2] ! = 4'b0000 || down_button[6:2] ! = 5'b00000 || inside_button[6:2] ! = 5'b00000)
                    nstate = s2_up;
                    
                    else if (up_button[0] == 1'b1 || inside_button[0] == 1'b1)
                    nstate = s1_down;
                    
                    else
                    nstate = s1_idle;
                    
                    
                end
                
                
                door_1:begin
                
                    reset_inside_button[1] = 1'b1;

                    if (dir_up == 1'b1) begin
                        reset_up[1] = 1'b1;
                        nstate      = s1_up;

                    end

                    else begin
                        reset_down[1] = 1'b1;
                        nstate        = s1_down;
                    end
                
                
                end
                
                
                
                
                s1_down:begin
                
                    present_floor          = 7'b0000010;
                    reset_inside_button[1] = 1'b0;
                    reset_down[1]          = 1'b0;

                    if (down_button[1] == 1'b1 || inside_button[1] == 1'b1) begin
                        dir_up = 1'b0;
                        nstate = door_1;
                    end
                
                    else if (up_button[0] == 1'b1 || inside_button[0] == 1'b1)
                    nstate = s0_up;
                
                    else if (up_button[5:2] ! = 4'b0000 || down_button[6:2] ! = 5'b00000 || inside_button[6:2] ! = 5'b00000)
                    nstate = s1_up;
                
                    else
                    nstate = s1_idle;
                    
                end



                
                //--------------------------second floor--------------------------------------------------------------
                
                
                s2_idle:begin
                
                    present_floor = 7'b0000100;
                
                    if (up_button ! = 7'b0000000 || down_button ! = 7'b0000000 || inside_button ! = 7'b0000000) begin
                        if (up_button[1:0] ! = 2'b00 || down_button[2:1] ! = 2'b00 || inside_button[1:0] ! = 2'b00)
                            nstate = s2_down;

                        else
                            nstate = s2_up;
                
                    end
                
                    else
                    nstate = s2_idle;

                end
                
                s2_up:begin
                
                    present_floor          = 7'b0000100;
                
                    reset_inside_button[2] = 1'b0;
                
                    reset_up[2]            = 1'b0;
                
                    if (up_button[2] == 1'b1 || inside_button[2] == 1'b1) begin
                        dir_up = 1'b1;
                        nstate = door_2;
                    end
                
                    else if (up_button[5:3] ! = 3'b000 || inside_button[6:3] ! = 4'b0000 || down_button[6:3] ! = 4'b0000)
                    nstate = s3_up;
                
                    else if (up_button[1:0] ! = 2'b00 || down_button[1] == 1'b1 || inside_button[1:0] ! = 2'b00)
                    nstate = s2_down;
                
                    else
                    nstate = s2_idle;

                end
                
                door_2:begin
                
                    reset_inside_button[2] = 1'b1;

                    if (dir_up == 1'b1) begin
                        reset_up[2] = 1'b1;
                        nstate      = s2_up;
                    end
                
                    else begin
                        reset_down[2] = 1'b1;
                        nstate        = s2_down;
                    end
                end
                
                s2_down:begin
                
                    present_floor          = 7'b0000100;

                    reset_inside_button[2] = 1'b0;

                    reset_down[2]          = 1'b0;
                
                    if (down_button[2] == 1'b1 || inside_button[2] == 1'b1) begin
                        dir_up = 1'b0;
                        nstate = door_2;
                    end

                    else if (up_button[1:0] ! = 2'b00 || down_button[1] == 1'b1 || inside_button[1:0] ! = 2'b00)
                    nstate = s1_down;
                    
                    else if (up_button[5:3] ! = 3'b000 || down_button[6:3] ! = 4'b0000 || inside_button[6:3] ! = 4'b0000)
                    nstate = s2_up;

                    else
                    nstate = s2_idle;
                    
                end




                //--------------------------3rd floor--------------------------------------------------------------
                
                s3_idle:begin

                    present_floor = 7'b0001000;

                    if (up_button ! = 7'b0000000 || down_button ! = 7'b0000000 || inside_button ! = 7'b0000000) begin
                
                        if (up_button[2:0] ! = "000" || down_button[3:1] ! = 3'b000 || inside_button[2:0] ! = 3'b000)
                            nstate = s3_down;

                        else
                            nstate = s3_up;
                    end
                
                    else
                    nstate = s3_idle;
                end
                
                s3_up:begin

                    present_floor          = 7'b0001000;

                    reset_inside_button[3] = 1'b0;

                    reset_up[3]            = 1'b0;
                
                    if (up_button[3] == 1'b1 || inside_button[3] == 1'b1) begin
                        dir_up = 1'b1;
                        nstate = door_3;
                    end
                
                    else if (up_button[5:4] ! = 2'b00 || inside_button[6:4] ! = 3'b000 || down_button[6:4] ! = 3'b000)
                    nstate = s4_up;
                
                    else if (up_button [2:0] ! = 3'b000 || down_button[2:1] ! = 2'b00 || inside_button[2:0] ! = 3'b000)
                    nstate = s3_down;
                
                    else
                    nstate = s3_idle;

                end
                
                s3_down:begin
                
                    present_floor          = 7'b0001000;

                    reset_inside_button[3] = 1'b0;

                    reset_down[3]          = 1'b0;
                
                    if (down_button[3] == 1'b1 || inside_button[3] == 1'b1) begin
                        dir_up = 1'b0;
                        nstate = door_3;
                    end
                
                    else if (up_button[2:0] ! = 3'b000 || down_button[2:1] ! = 2'b00 || inside_button[2:0] ! = 3'b000)
                    nstate = s2_down;
                
                    else if (up_button[5:4] ! = 2'b00 || inside_button[6:4] ! = 3'b000 || down_button[6:4] ! = 3'b000)
                    nstate = s3_up;
                
                    else
                    nstate = s3_idle;

                end
                
                door_3:begin
                
                    reset_inside_button[3] = 1'b1;

                    if (dir_up == 1'b1) begin
                        reset_up[3] = 1'b1;
                        nstate      = s3_up;
                    end
                
                    else begin
                        reset_down[3] = 1'b1;
                        nstate        = s3_down;
                    end

                end



                //--------------------------4th floor--------------------------------------------------------------
                s4_idle:begin
                
                    present_floor = 7'b0010000;

                    if (up_button ! = 7'b0000000 || down_button ! = 7'b0000000 || inside_button ! = 7'b0000000) begin
                        if (up_button[5] == 1'b1 || down_button[6:5] ! = 2'b00 || inside_button[6:5] ! = 2'b00 || up_button[4] == 1'b1)
                            nstate = s4_up;

                        else
                            nstate = s4_down;
                    end
                
                    else
                    nstate = s4_idle;

                end
                
                s4_up:begin
                
                    present_floor          = 7'b0010000;

                    reset_inside_button[4] = 1'b0;

                    reset_up[4]            = 1'b0;

                    if (up_button[4] == 1'b1 || inside_button[4] == 1'b1) begin
                        dir_up = 1'b1;
                        nstate = door_4;
                    end
                
                    else if (up_button[5] ! = 1'b0 || inside_button[6:5] ! = 2'b00 || down_button[6:5] ! = 2'b00)
                    nstate = s5_up;
                
                    else if (up_button[3:0] ! = 4'b0000 || down_button[3:1] ! = 3'b000 || inside_button[3:0] ! = 4'b0000)
                    nstate = s4_down;
                
                    else
                    nstate = s4_idle;

                end
                
                s4_down:begin
                
                    present_floor          = 7'b0010000;

                    reset_inside_button[4] = 1'b0;

                    reset_down[4]          = 1'b0;

                    if (down_button[4] == 1'b1 || inside_button[4] == 1'b1) begin
                        dir_up = 1'b0;
                        nstate = door_4;
                    end
                
                    else if (up_button [3:0] ! = 4'b0000 || down_button[3:1] ! = 3'b000 || inside_button[3:0] ! = 4'b0000)
                    nstate = s3_down;
                
                    else if (up_button[5] == 1'b1 || inside_button[6:5] ! = 2'b00 || down_button[6:5] ! = 2'b00)
                    nstate = s4_up;
                    
                    else
                    nstate = s4_idle;

                end
                
                door_4:begin
                
                    reset_inside_button[4] = 1'b1;
                    if (dir_up == 1'b1) begin
                        reset_up[4] = 1'b1;
                        nstate      = s4_up;
                    end
                
                    else begin
                        reset_down[4] = 1'b1;
                        nstate        = s4_down;
                
                    end
                end
                //--------------------------5th floor--------------------------------------------------------------
                
                s5_idle:begin
                
                    present_floor = 7'b0100000;

                    if (up_button ! = 7'b0000000 || down_button ! = 7'b0000000 || inside_button ! = 7'b0000000) begin
                        if (down_button[6] == 1'b1 || inside_button[6] == 1'b1 || up_button[5] == 1'b1)
                            nstate = s5_up;

                        else
                            nstate = s5_down;
                    end
                
                    else
                    nstate = s5_idle;

                end
                
                s5_up:begin
                
                    present_floor          = 7'b0100000;

                    reset_inside_button[5] = 1'b0;

                    reset_up[5]            = 1'b0;
                
                    if (up_button[5] == 1'b1 || inside_button[5] == 1'b1) begin
                        dir_up = 1'b1;
                        nstate = door_5;
                    end
                
                    else if (down_button[6] == 1'b1 || inside_button[6] == 1'b1)
                    nstate = s6_down;
                
                    else if (up_button[4:0] ! = 5'b00000 || down_button[4:1] ! = 4'b0000 || inside_button[4:0] ! = 4'b0000)
                    nstate = s5_down;
                
                    else
                    nstate = s5_idle;

                end
                
                s5_down:begin
                
                    present_floor          = 7'b0100000;

                    reset_inside_button[5] = 1'b0;

                    reset_down[5]          = 1'b0;

                    if (down_button[5] == 1'b1 || inside_button[5] == 1'b1) begin
                        dir_up = 1'b0;
                        nstate = door_5;
                    end
                
                    else if (up_button[4:0] ! = 5'b00000 || down_button[4:1] ! = 4'b0000 || inside_button[4:0] ! = 4'b0000)
                    nstate = s4_down;
                
                    else if (down_button[6] == 1'b1 || inside_button[6] == 1'b1)
                    nstate = s5_up;
                    
                    else
                    nstate = s5_idle;

                end
                
                door_5:begin
                
                    reset_inside_button[5] = 1'b1;

                    if (dir_up == 1'b1) begin
                        reset_up[5] = 1'b1;
                        nstate      = s5_up;
                    end
                
                    else begin
                        reset_down[5] = 1'b1;
                        nstate        = s5_down;
                    end

                end
                


                //--------------------------6th floor--------------------------------------------------------------
                
                s6_idle:begin
                
                    present_floor = 7'b1000000;
                
                    if (up_button ! = 7'b0000000 || down_button ! = 7'b0000000 || inside_button ! = 7'b0000000)
                    nstate = s6_down;
                    
                    else
                    nstate = s6_idle;

                end
                
                s6_down:begin

                    present_floor          = 7'b1000000;

                    reset_inside_button[6] = 1'b0;

                    reset_down[6]          = 1'b0;
                
                    if (down_button[6] == 1'b1 || inside_button[6] == 1'b1) begin
                        dir_up = 1'b0;
                        nstate = door_5;
                    end
                
                    else if (up_button [5:0] ! = 6'b000000 || down_button[5:1] ! = 5'b00000 || inside_button[5:0] ! = 6'b000000)
                    nstate = s5_down;
                
                    else
                    nstate = s6_idle;

                end
                
                door_6:begin
                
                    reset_inside_button[6] = 1'b1;
                    reset_down[6]          = 1'b1;
                    nstate                 = s6_down;
                end
            endcase
            
        end
    end
    
    
    
    
    

    
endmodule