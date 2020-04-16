# Test file for "Lab3"


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

# Example test:
test "PIND: 0x00 => PORTB: 0x00"
# Set inputs
setPIND 0x00
# Continue for several ticks
continue 2
# Set expect values
expectPORTB 0x00
# Check pass/fail
checkResult

# Add tests below
test "PIND: 0x80 => PORTB: 0x02"
setPIND 0x80
continue 2
expectPORTB 0x02
checkResult

test "PIND: 0x04, PINB: 0x01 => PORTB: 0x00"
setPIND 0x04
setPINB 0x01
continue 2
expectPORTB 0x00
checkResult

test "PIND: 0x2A => PORTB: 0x04"
setPIND 0x2A
continue 2
expectPORTB 0x04
checkResult

test "PIND: 0x46 => PORTB: 0x02"
setPIND 0x46
continue 2
expectPORTB 0x02
checkResult

test "PIND: 0x45, PINB: 0x01 => PORTB: 0x02"
setPIND 0x45
setPINB 0x01
continue 2
expectPORTB 0x02
checkResult

test "PIND: 0x05, PINB: 0x01 => PORTB: 0x04"
setPIND 0x05
setPINB 0x01
continue 2
expectPORTB 0x04
checkResult

test "PIND: 0xFF, PINB: 0x01 => PORTB: 0x02"
setPIND 0xFF
setPINB 0x01
continue 2
expectPORTB 0x02
checkResult

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
