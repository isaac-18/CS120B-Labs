//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i
static unsigned char amplitude;
static unsigned char i;

/*complete the state machine*/

void Detect_EQ()
{
    switch(detect_eq_state)
    {
        case DEQInit:
	    i = 0;
	    detect_eq_state = DEQ_Wait;
            break;

	case DEQ_Wait:
	    if (amplitude > 0) {
	    	detect_eq_state = DEQ_Detected;
		i = 0;
	    }
	    else {
	    	detect_eq_state = DEQ_Wait;
	    }
	    break;

	case DEQ_Detected: 
	   amplitude = ~PINA & 0xF8;
	   if (amplitude > 0) {
	   	detect_eq_state = DEQ_Detected;
	   }
	   else if ((amplitude <= 0) && (i < 10)) {
		++i;
	        if (i >= 10) {
	       	    detect_eq_state = DEQ_Wait;
	        }
	   }
	   break;

        default:
            detect_eq_state = DEQInit;
            break;
    }
    switch(detect_eq_state)
    {
        case DEQInit:
	    ampOut = 0x00;	// Global var for max amp to read
            break;

	case DEQ_Wait:
	    amplitude = ~PINA & 0xF8;
	    eq_detected = 0;
	    ampOut = 0x00;	// Might need to change this so max val is held
	    break;

	case DEQ_Detected:
	    eq_detected = 1;
	    ampOut = amplitude;
	    break;

        default:
            break;
    }
}
