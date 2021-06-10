#!/bin/bash

OFFSET=0x400
EXPECTED_HEX="414903"

RESULT=$(xxd -seek $OFFSET -l 3 -ps $1)

if [ "$EXPECTED_HEX" = "$RESULT" ]; then
    echo "Magic bytes (0x$EXPECTED_HEX) found at offset $OFFSET."
    exit 0
else
    echo "Missing magic bytes (0x$EXPECTED_HEX) at offset $OFFSET."
    exit 1
fi
