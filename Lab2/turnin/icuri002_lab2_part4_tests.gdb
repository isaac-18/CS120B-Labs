# Test file for "Lab2"


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
test "PINA: 0x00, PINB: 0x00, PINC: 0x00 => PORTD: 0x00"
# Set inputs
setPINA 0x00
setPINB 0x00
setPINC 0x00
# Continue for several ticks
continue 5
# Set expect values
expectPORTD 0x00
# Check pass/fail
checkResult

# Add tests below
test "PINA: 0xFF, PINB: 0x00, PINC: 0x00 => PORTD: 0xFF"
setPINA 0xFF
setPINB 0x00
setPINC 0x00
continue 5
expectPORTD 0xFF
checkResult

test "PINA: 0xF0, PINB: 0x00, PINC: 0x00 => PORTD: 0xF3"
setPINA 0xF0
setPINB 0x00
setPINC 0x00
continue 5
expectPORTD 0xF3
checkResult

test "PINA: 0x00, PINB: 0x0F, PINC: 0x00 => PORTD: 0x0C (+- 3)"
setPINA 0x00
setPINB 0x0F
setPINC 0x00
continue 5
expectPORTD 0x0C
checkResult

test "PINA: 0x14, PINB: 0x0F, PINC: 0x20 => PORTD: 0x40"
setPINA 0x14
setPINB 0x0F
setPINC 0x20
continue 5
expectPORTD 0x40
checkResult

test "PINA: 0x01, PINB: 0x00, PINC: 0x01 => PORTD: 0x00"
setPINA 0x01
setPINB 0x00
setPINC 0x01
continue 5
expectPORTD 0x00
checkResult
# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
