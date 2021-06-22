#!/bin/sh

SUCCESS_COLOR="\033[01;32m"
ERROR_COLOR="\033[01;31m"
NORMAL_COLOR="\033[0m"

norminette src main.c > norminette_result

NORM_ERROR=$(grep -c 'Error!' norminette_result)

if [[ "$NORM_ERROR" > 0 ]]; then
    printf "$ERROR_COLOR NORM ERROR :(\n"
    printf "$NORMAL_COLOR incorrect files: [$ERROR_COLOR $NORM_ERROR $NORMAL_COLOR]\n"
else
    printf "$SUCCESS_COLOR NORM IS OK :)\n"
fi

exit $NORM_ERROR