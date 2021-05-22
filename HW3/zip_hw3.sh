#!/bin/bash

ID1='315836569'
ID2='307866269'
ZIP_NAME='$(ID1)-$(ID2).zip'

C_FILES='*.c'
_PP_FILES='*.%pp'
CXX_FILES='*.cxx'
HEADERS='*.h*'
LEX_FILE='scanner.lex'

# todo: exclude 'hw3', flex files and bison files.

FILES='$(C_FILES) $(_PP_FILES) $(CXX_FILES) $(HEADERS) $(LEX_FILE)'

zip $(ZIP_NAME) $(FILES)
