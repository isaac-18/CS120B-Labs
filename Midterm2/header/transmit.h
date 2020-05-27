//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i
static unsigned char B;


/*complete the state machine*/

void Transmit()
{
    switch(transmit_state)
    {
        case TInit:
	    transmit_state = Output;
            break;
	
	case Output:
	    transmit_state = Output;
	    break;

        default:
            transmit_state = TInit;
            break;
    }
    switch(transmit_state)
    {
        case TInit:
            break;

	case Output:
	    B = ampOut | (eq_detected << 1) | pingOut;
	    break;

        default:
            break;
    }
    PORTB = B;
}
