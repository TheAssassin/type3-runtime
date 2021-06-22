#!/bin/bash

TARGET_FILE=$1

function check_bytes() {
  FIELD_NAME=$1
  EXPECTED_HEX=$2
  EXPECTED_LEN=$(expr ${#EXPECTED_HEX} / 2)
  OFFSET=$3


  FILE_SECTION=$(xxd -seek $OFFSET -l $EXPECTED_LEN  -ps $TARGET_FILE)

  if [ "$EXPECTED_HEX" = "$FILE_SECTION" ]; then
    echo "$FIELD_NAME (0x$EXPECTED_HEX) found at offset $OFFSET."
  else
    echo "Wrong $FIELD_NAME (0x$EXPECTED_HEX) at offset $OFFSET. Found $FILE_SECTION"
  exit 1
fi
}

check_bytes "Magic Bytes" "414903" 0x400
check_bytes "Header Revision" "0100" 0x403
check_bytes "Payload Format" "0100" 0x405
check_bytes "Payload Offset" "aaaaaaaa00000000" 0x407
check_bytes "Signature Offset" "aaaaaaaa00000000" 0x40F