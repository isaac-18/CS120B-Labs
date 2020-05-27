
//define local variable(s) here.  Use static keyword to keep local, e.g:
//   static int i;  // defines a local int named i
static unsigned char i;
static unsigned char H;
static unsigned char L;

/*complete the state machine*/

void Ping()
{
    switch(ping_state)
    {
        case PInit:
	    ping_state = PingHigh;
	    i = 0;
            break;

	case PingHigh:
	    if (!(i < H)) {
	    	ping_state = PingLow;
		i = 0;
	    }
	    else if (i < H) {
	    	ping_state = PingHigh;
	    }
	    break;

	case PingLow:
	    if (!(i < L)) {
	    	ping_state = PingHigh;
		i = 0;
	    }
	    else if (i < L) {
	    	ping_state = PingLow;
	    }
	    break;

        default:
            ping_state = PInit;
            break;
    }

    switch(ping_state)
    {
        case PInit:
            pingOut = 0;	// Global shared variable
	    H = 1;
	    L = 9;
            break;

	case PingHigh:
	    pingOut = 0x01;
	    ++i;
	    break;

	case PingLow:
	    pingOut = 0x00;
	    ++i;
	    break;

        default:
            break;
    }
}
